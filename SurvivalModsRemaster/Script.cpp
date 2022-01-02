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
std::vector<bool> TriggerPedsData::killedFlags;
std::vector<int> TriggerPedsData::starTime;
std::vector<std::string> tasks;
int Data::intermissionDuration;
Controls Data::tenWaveControl;
Controls Data::infiniteWaveControl;
Controls Data::timedSurvivalControl;
Controls Data::cancelControl;
Controls Data::reloadTriggerPedsControl;
Controls Data::hardcoreSurvivalControl;
bool Data::showControls;
bool canStart;
int cancelStartTime;
int cancelCurrentTime;
Hash Data::enemiesRelGroup;
Hash Data::neutralRelGroup;
bool playerDied;
bool initialized = false;
nlohmann::json j;
std::string startSurvivalText;
int Data::TPIndex;

void IsPlayerInMissionStartRange()
{
    if (SURVIVAL::SurvivalData::IsActive || PLAYER::GET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID()) > 1 
        || GAMEPLAY::GET_MISSION_FLAG())
    {
        Data::showControls = false;
        canStart = false;
        return;
    }

    Vector3 playerPosition = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
    size_t size = TriggerPedsData::peds.size();

    for (int i = 0; i < size; i++)
    {
        Vector3 survCoords = TriggerPedsData::positions.at(i).coords;

        if (CALC::IsInRange_2(playerPosition, TriggerPedsData::positions.at(i).coords, 80.0f))
        {
            Data::TPIndex = i;
            canStart = true;

            Ped ped = TriggerPedsData::peds.at(i);

            if (ped == 0)
            {
                Data::showControls = false;
                return;
            }

            Vector3 coords = ENTITY::GET_ENTITY_COORDS(ped, false);
            Data::showControls = CALC::IsInRange_2(playerPosition, coords, 4.5f) && !PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false);
            return;
        }
    }

    Data::showControls = false;
    canStart = false;
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

    Hash playerGroupHash = GAMEPLAY::GET_HASH_KEY((char*)"PLAYER");
    Hash cougarGroupHash = GAMEPLAY::GET_HASH_KEY((char*)"COUGAR");
    PED::ADD_RELATIONSHIP_GROUP((char*)"SURVIVAL_MISSION_ENEMIES_REL_GROUP", &Data::enemiesRelGroup);
    PED::ADD_RELATIONSHIP_GROUP((char*)"SURVIVAL_MISSION_TRIGGER_REL_GROUP", &Data::neutralRelGroup);
    PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, Data::enemiesRelGroup, playerGroupHash);
    PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, playerGroupHash, Data::enemiesRelGroup);
    PED::SET_RELATIONSHIP_BETWEEN_GROUPS(0, Data::enemiesRelGroup, cougarGroupHash);
    PED::SET_RELATIONSHIP_BETWEEN_GROUPS(0, cougarGroupHash, Data::enemiesRelGroup);
    PED::SET_RELATIONSHIP_BETWEEN_GROUPS(3, Data::neutralRelGroup, playerGroupHash);
    PED::SET_RELATIONSHIP_BETWEEN_GROUPS(3, playerGroupHash, Data::neutralRelGroup);

    INIT::LoadTriggerPeds();
}

void ControlsWatch()
{
    if (!SURVIVAL::SurvivalData::IsActive && !canStart)
    {
        if (CONTROLS::IS_CONTROL_JUST_PRESSED(0, static_cast<int>(Data::reloadTriggerPedsControl)))
        {
            INIT::LoadTriggerPeds();
        }
    }

    if (Data::showControls) 
    {
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
    else if (SURVIVAL::SurvivalData::IsActive)
    {
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

bool KilledByPlayer(Ped ped)
{
    int player = PLAYER::PLAYER_PED_ID();
    Entity killer = PED::_GET_PED_KILLER(ped);

    if (killer == player || killer == 0)
        return true;

    return false;
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
                bool inRange = CALC::IsInRange_2(playerPosition, TPPos.coords, 80.0f);
                Ped ped = TriggerPedsData::peds.at(i);

                if (ped == 0 && canStart)
                {
                    if (inRange && !TriggerPedsData::killedFlags.at(i))
                        TriggerPedsData::peds.at(i) = INIT::SpawnTriggerPed(i);
                    else if (!inRange)
                        TriggerPedsData::killedFlags.at(i) = false;
                }
                else if (ped != 0)
                {
                    if (!canStart)
                    {
                        ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&ped);
                        TriggerPedsData::peds.at(i) = 0;
                        TriggerPedsData::killedFlags.at(i) = true;
                    }
                    else if (!inRange)
                    {
                        ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&ped);
                        TriggerPedsData::peds.at(i) = 0;
                    }
                    else if (ENTITY::IS_ENTITY_DEAD(ped))
                    {
                        if (KilledByPlayer(ped))
                        {
                            UI::REMOVE_BLIP(&TriggerPedsData::blips.at(i));
                            TriggerPedsData::blips.at(i) = 0;
                            ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&ped);
                            TriggerPedsData::peds.at(i) = 0;
                            SURVIVAL::StartMission(false, false, false);
                        }
                        else
                        {
                            ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&ped);
                            TriggerPedsData::peds.at(i) = 0;
                            TriggerPedsData::killedFlags.at(i) = true;
                        }
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
        IsPlayerInMissionStartRange();
        ProcessTriggerPeds();

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
