#include "pch.h"
#include "Script.hpp"
#include "json.hpp"

int Data::intermissionDuration;
std::map<std::string, Ped> Data::missionTriggerPeds;
std::string Data::currentPedKey;
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
bool Data::showPassedScaleform;
bool playerDied;
bool initialized = false;
nlohmann::json j;
std::string startSurvivalText;
bool passedScaleformRequested;
bool passedScaleformFading;
int passedScaleformST;
int passedScaleform;

void IsPlayerInMissionStartRange()
{
    if (SURVIVAL::SurvivalData::IsActive || PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false))
    {
        Data::canStartMission = false;
        return;
    }

    Vector3 playerPosition = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
    for (const auto& [key, value] : Data::missionTriggerPeds)
    {
        Vector3 pedPosition = ENTITY::GET_ENTITY_COORDS(value, true);
        if (CALC::IsInRange_2(playerPosition, pedPosition, 4.5f))
        {
            Data::canStartMission = true;
            Data::currentPedKey = key;
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

    Data::missionTriggerPeds = INIT::SpawnMissionTriggerPeds();
}

void ControlsWatch()
{
    if (!SURVIVAL::SurvivalData::IsActive && !Data::canStartMission)
    {
        if (CONTROLS::IS_CONTROL_JUST_RELEASED(0, static_cast<int>(Data::reloadTriggerPedsControl)))
        {
            STORAGE::StoredData::peds.clear();
            STORAGE::StoredData::timers.clear();

            for (const auto& [key, value] : Data::missionTriggerPeds)
            {
                Ped ped = value;
                PED::DELETE_PED(&ped);
            }

            Data::missionTriggerPeds.clear();
            Data::missionTriggerPeds = INIT::SpawnMissionTriggerPeds();
        }
    }

    if (Data::canStartMission) {
        if (CONTROLS::IS_CONTROL_JUST_RELEASED(0, static_cast<int>(Data::tenWaveControl)))
        {
            SURVIVAL::StartMission(false, false, false);
        }

        if (CONTROLS::IS_CONTROL_JUST_RELEASED(0, static_cast<int>(Data::infiniteWaveControl)))
        {
            SURVIVAL::StartMission(true, false, false);
        }

        if (CONTROLS::IS_CONTROL_JUST_RELEASED(0, static_cast<int>(Data::timedSurvivalControl)))
        {
            SURVIVAL::StartMission(false, true, false);
        }

        if (CONTROLS::IS_CONTROL_JUST_RELEASED(0, static_cast<int>(Data::hardcoreSurvivalControl)))
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

void ProcessDelayedSpawns()
{
    for (int i = 0; i < STORAGE::StoredData::peds.size(); i++)
    {
        SpawnData pedData = STORAGE::StoredData::peds.at(i);
        
        if (!pedData.timerActive)
        {
            continue;
        }

        if (!STORAGE::StoredData::timers.at(i).SpawnTimer())
        {
            continue;
        }

        Ped handle = INIT::SpawnTriggerPed(pedData.stringName);
        Data::missionTriggerPeds.insert(std::pair<std::string, Ped>(pedData.stringName, handle));
        STORAGE::StoredData::peds.erase(STORAGE::StoredData::peds.begin() + i);
        STORAGE::StoredData::timers.erase(STORAGE::StoredData::timers.begin() + i);

        break;
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
        ProcessDelayedSpawns();
        IsPlayerInMissionStartRange();

        if (Data::showPassedScaleform)
        {
            if (!passedScaleformRequested)
            {
                passedScaleform = SCREEN::RequestScaleform();
                SCREEN::SetScaleformText(passedScaleform, "~y~survival passed", "");
                passedScaleformST = GAMEPLAY::GET_GAME_TIMER();
                passedScaleformRequested = true;
            }

            GRAPHICS::DRAW_SCALEFORM_MOVIE_FULLSCREEN(passedScaleform, 255, 255, 0, 255, 0);

            if (GAMEPLAY::GET_GAME_TIMER() - passedScaleformST >= 8000 && !passedScaleformFading)
            {
                SCREEN::FadeOutScaleform(passedScaleform, 2000);
                passedScaleformFading = true;
            }
            else if (GAMEPLAY::GET_GAME_TIMER() - passedScaleformST >= 2000 && passedScaleformFading)
            {
                SCREEN::FreeScaleform(passedScaleform);
                passedScaleformRequested = false;
                passedScaleformFading = false;
                Data::showPassedScaleform = false;
            }
        }

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

    for (const auto& [key, value] : Data::missionTriggerPeds)
    {
        ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&Data::missionTriggerPeds.at(key));
    }

    Data::missionTriggerPeds.clear();
    STORAGE::StoredData::peds.clear();
    STORAGE::StoredData::timers.clear();

    if (SURVIVAL::SurvivalData::IsActive)
    {
        SURVIVAL::ScriptQuit();
    }
}
