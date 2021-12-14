#include "pch.h"
#include "Script.hpp"
#include "json.hpp"

int Data::intermissionDuration;
std::map<std::string, Ped> Data::missionTriggerPeds;
std::string Data::currentPedKey;
Controls tenWaveControl;
Controls infiniteWaveControl;
Controls timedSurvivalControl;
Controls cancelControl;
Controls reloadTriggerPedsControl;
static bool canStartMission = false;
int cancelStartTime;
int cancelCurrentTime;
Hash Data::enemiesRelGroup;
bool playerDied;
bool initialized = false;
nlohmann::json j;
std::string startSurvivalText;

void ShowStartSurvivalText()
{
	SCREEN::ShowHelpTextThisFrame(startSurvivalText.c_str(), true);
}

void IsPlayerInMissionStartRange()
{
    if (SURVIVAL::SurvivalData::IsActive || PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false))
    {
        canStartMission = false;
        return;
    }

    Vector3 playerPosition = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
    for (const auto& [key, value] : Data::missionTriggerPeds)
    {
        Vector3 pedPosition = ENTITY::GET_ENTITY_COORDS(value, true);
        if (CALC::IsInRange_2(playerPosition, pedPosition, 4.5f))
        {
            canStartMission = true;
            Data::currentPedKey = key;
            return;
        }
    }
    canStartMission = false;
}

void ReadConfigAndSpawnTriggerPeds()
{
    std::ifstream i("SurvivalsData\\config.json");
    i >> j;
    i.close();
    int tenControl = j["Controls"]["StartTenWave"];
    int infiniteControl = j["Controls"]["StartInfiniteWaves"];
    int timedControl = j["Controls"]["StartTimedSurvival"];
    int cControl = j["Controls"]["CancelSurvival"];
    Data::intermissionDuration = j["Gameplay"]["IntermissionDuration"];

    tenWaveControl = static_cast<Controls>(j["Controls"]["StartTenWave"]);
    infiniteWaveControl = static_cast<Controls>(j["Controls"]["StartInfiniteWaves"]);
    timedSurvivalControl = static_cast<Controls>(j["Controls"]["StartTimedSurvival"]);
    cancelControl = static_cast<Controls>(j["Controls"]["CancelSurvival"]);
    reloadTriggerPedsControl = static_cast<Controls>(j["Controls"]["ReloadTriggerPeds"]);

    char evName[500];
    strcpy_s(evName, "~");
    strcat_s(evName, controlsNames[tenControl]);
    strcat_s(evName, "~ Ten waves\n~");
    strcat_s(evName, controlsNames[infiniteControl]);
    strcat_s(evName, "~ Infinite waves\n~");
    strcat_s(evName, controlsNames[timedControl]);
    strcat_s(evName, "~ Timed");

    startSurvivalText = evName;

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
    if (!SURVIVAL::SurvivalData::IsActive && !canStartMission)
    {
        if (CONTROLS::IS_CONTROL_JUST_RELEASED(0, static_cast<int>(reloadTriggerPedsControl)))
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

    if (canStartMission) {
        if (CONTROLS::IS_CONTROL_JUST_RELEASED(0, static_cast<int>(tenWaveControl)))
        {
            SURVIVAL::StartMission(false, false);
        }

        if (CONTROLS::IS_CONTROL_JUST_RELEASED(0, static_cast<int>(infiniteWaveControl)))
        {
            SURVIVAL::StartMission(true, false);
        }

        if (CONTROLS::IS_CONTROL_JUST_RELEASED(0, static_cast<int>(timedSurvivalControl)))
        {
            SURVIVAL::StartMission(false, true);
        }

    }
    else if (SURVIVAL::SurvivalData::IsActive) {
        if (CONTROLS::IS_CONTROL_JUST_PRESSED(0, static_cast<int>(cancelControl)))
        {
            cancelStartTime = GAMEPLAY::GET_GAME_TIMER();
        }

        if (CONTROLS::IS_CONTROL_PRESSED(0, static_cast<int>(cancelControl)))
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

        if (!SURVIVAL::SurvivalData::IsActive)
        {
            if (canStartMission)
            {
                ShowStartSurvivalText();
            }
        }
        else
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
	srand(GetTickCount());
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
