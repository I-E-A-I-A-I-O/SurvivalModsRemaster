#include "pch.h"
#include "Script.hpp"
#include "json.hpp"

std::vector<std::string> TriggerPedsData::names;
std::vector<std::string> TriggerPedsData::models;
std::vector<std::string> TriggerPedsData::tasks;
std::vector<Ped> TriggerPedsData::peds;
std::vector<EntityPosition> TriggerPedsData::positions;
std::vector<Blip> TriggerPedsData::blips;
std::vector<bool> TriggerPedsData::timerActive;
std::vector<int> TriggerPedsData::starTime;
std::vector<std::string> tasks;
int Data::intermissionDuration;
Controls Data::tenWaveControl;
Controls Data::infiniteWaveControl;
Controls Data::timedSurvivalControl;
Controls Data::cancelControl;
Controls Data::reloadTriggerPedsControl;
Controls Data::hardcoreSurvivalControl;
bool Data::canStartMission;
int cancelStartTime;
int cancelCurrentTime;
Hash Data::enemiesRelGroup;
bool playerDied;
bool initialized = false;
nlohmann::json j;
std::string startSurvivalText;
int Data::TPIndex;

void IsPlayerInMissionStartRange()
{
    if (SURVIVAL::SurvivalData::IsActive || PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false))
    {
        Data::canStartMission = false;
        return;
    }

    Vector3 playerPosition = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
    size_t size = TriggerPedsData::peds.size();

    for (int i = 0; i < size; i++)
    {
        Ped ped = TriggerPedsData::peds.at(i);
        
        if (ped == 0)
            continue;

        Vector3 coords = ENTITY::GET_ENTITY_COORDS(ped, false);

        if (CALC::IsInRange_2(playerPosition, coords, 4.5f))
        {
            Data::canStartMission = true;
            Data::TPIndex = i;
            return;
        }
    }

    Data::canStartMission = false;
}

void ReadConfigAndSpawnTriggerPeds()
{
    std::ifstream i("SurvivalsData\\config.json");
    i >> j;
    i.close();

    Data::intermissionDuration = j["Gameplay"]["IntermissionDuration"];
    Data::tenWaveControl = static_cast<Controls>(j["Controls"]["StartTenWave"]);
    Data::infiniteWaveControl = static_cast<Controls>(j["Controls"]["StartInfiniteWaves"]);
    Data::timedSurvivalControl = static_cast<Controls>(j["Controls"]["StartTimedSurvival"]);
    Data::cancelControl = static_cast<Controls>(j["Controls"]["CancelSurvival"]);
    Data::reloadTriggerPedsControl = static_cast<Controls>(j["Controls"]["ReloadTriggerPeds"]);
    Data::hardcoreSurvivalControl = static_cast<Controls>(j["Controls"]["StartHardcoreSurvival"]);

    char relName[] = "SURVIVAL_MISSION_ENEMIES_REL_GROUP";
    char playerRelName[] = "PLAYER";
    char cougarRelName[] = "COUGAR";
    Hash playerGroupHash = GAMEPLAY::GET_HASH_KEY(playerRelName);
    Hash cougarGroupHash = GAMEPLAY::GET_HASH_KEY(cougarRelName);

    PED::ADD_RELATIONSHIP_GROUP(relName, &Data::enemiesRelGroup);
    PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, Data::enemiesRelGroup, playerGroupHash);
    PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, playerGroupHash, Data::enemiesRelGroup);
    PED::SET_RELATIONSHIP_BETWEEN_GROUPS(0, Data::enemiesRelGroup, cougarGroupHash);
    PED::SET_RELATIONSHIP_BETWEEN_GROUPS(0, cougarGroupHash, Data::enemiesRelGroup);

    INIT::LoadTriggerPeds();
}

void ControlsWatch()
{
    if (!SURVIVAL::SurvivalData::IsActive && !Data::canStartMission)
    {
        if (CONTROLS::IS_CONTROL_JUST_PRESSED(0, static_cast<int>(Data::reloadTriggerPedsControl)))
        {
            INIT::LoadTriggerPeds();
        }
    }

    if (Data::canStartMission) {
        if (CONTROLS::IS_CONTROL_JUST_PRESSED(0, static_cast<int>(Data::tenWaveControl)))
        {
            SURVIVAL::StartMission(false, false, false);
        }

        if (CONTROLS::IS_CONTROL_JUST_PRESSED(0, static_cast<int>(Data::infiniteWaveControl)))
        {
            SURVIVAL::StartMission(true, false, false);
        }

        if (CONTROLS::IS_CONTROL_JUST_PRESSED(0, static_cast<int>(Data::timedSurvivalControl)))
        {
            SURVIVAL::StartMission(false, true, false);
        }

        if (CONTROLS::IS_CONTROL_JUST_PRESSED(0, static_cast<int>(Data::hardcoreSurvivalControl)))
        {
            SURVIVAL::StartMission(false, false, true);
        }
    }
    else if (SURVIVAL::SurvivalData::IsActive) {
        if (CONTROLS::IS_CONTROL_JUST_PRESSED(0, static_cast<int>(Data::cancelControl)))
        {
            cancelStartTime = GAMEPLAY::GET_GAME_TIMER();
        }

        if (CONTROLS::IS_CONTROL_PRESSED(0, static_cast<int>(Data::cancelControl)))
        {
            cancelCurrentTime = GAMEPLAY::GET_GAME_TIMER();
            if (cancelCurrentTime - cancelStartTime >= 3000)
            {
                SURVIVAL::QuitSurvival(false);
            }
        }
    }
}

void ProcessTriggerPeds()
{
    size_t size = TriggerPedsData::timerActive.size();

    for (size_t i = 0; i < size; i++)
    {
        bool active = TriggerPedsData::timerActive.at(i);
        
        if (!active)
        {
            if (!SURVIVAL::SurvivalData::IsActive)
            {
                Vector3 playerPosition = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
                EntityPosition TPPos = TriggerPedsData::positions.at(i);
                bool inRange = CALC::IsInRange_2(playerPosition, TPPos.coords, 150.0f);
                Ped ped = TriggerPedsData::peds.at(i);

                if (ped == 0 && inRange)
                {
                    TriggerPedsData::peds.at(i) = INIT::SpawnTriggerPed(i);
                }
                else if (ped != 0 && !inRange)
                {
                    if (ENTITY::IS_ENTITY_A_MISSION_ENTITY(ped))
                    {
                        ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&ped);
                        TriggerPedsData::peds.at(i) = 0;
                    }
                }
            }

            continue;
        }

        if (GAMEPLAY::GET_GAME_TIMER() - TriggerPedsData::starTime.at(i) < 300000)
        {
            continue;
        }

        TriggerPedsData::timerActive.at(i) = false;
        TriggerPedsData::blips.at(i) = BLIPS::CreateForMissionTriggerPed(TriggerPedsData::positions.at(i).coords, TriggerPedsData::names.at(i).c_str());
    }
}

void main()
{
    while (DLC2::GET_IS_LOADING_SCREEN_ACTIVE())
    {
        WAIT(0);
    }

    if (SURVIVAL::SurvivalData::IsActive)
    {
        OnAbort();
    }

    ReadConfigAndSpawnTriggerPeds();

    while (true)
    {
        ProcessTriggerPeds();
        IsPlayerInMissionStartRange();

        if (SURVIVAL::SurvivalData::IsActive)
        {
            if (SURVIVAL::SurvivalData::Triggered)
            {
                SURVIVAL::SetOffTrigger();
            }
            else if (SURVIVAL::SurvivalData::Started)
            {
                SURVIVAL::ProcessSurvival();

                if (PLAYER::IS_PLAYER_DEAD(PLAYER::PLAYER_ID()))
                {
                    SURVIVAL::QuitSurvival(true);
                }
            }
        }

        ControlsWatch();

        WAIT(0);
    }
}

void ScriptMain()
{
    srand(GetTickCount64());
	main();
}

void OnAbort()
{
    PLAYER::SET_PLAYER_CONTROL(PLAYER::PLAYER_ID(), true, 0);
    AI::CLEAR_PED_TASKS(PLAYER::PLAYER_PED_ID());
    PLAYER::SET_DISPATCH_COPS_FOR_PLAYER(PLAYER::PLAYER_ID(), true);
    TriggerPedsData::ClearTriggerPeds();

    if (SURVIVAL::SurvivalData::IsActive)
    {
        SURVIVAL::ScriptQuit();
    }
}
