#pragma once

#include "natives.h"
#include "enums.h"
#include "SurvivalSpawner.hpp"
#include "Script.hpp"
#include "Music.hpp"
#include "MissionTimers.hpp"
#include "Blip.hpp"
#include "Screen.hpp"
#include "Calculations.hpp"
#include "UIScript.hpp"

namespace SURVIVAL
{
	struct SurvivalData
	{
        static bool IsActive;
        static std::string MissionID;
        static bool InfiniteWaves;
        static int earnedMoney;
        static int CurrentWave;
        static int MaxAircraft;
        static int MaxVehicles;
        static int MaxWaveSize;
        static bool Started;
        static bool timed;
        static int timedTimeLeft;
        static bool cheated;
        static bool hardcore;
	};

    void StartMission(bool infiniteWaves, bool timed, bool hardcore);
    void Initialize();
    void ProcessSurvival();
    void CompleteSurvival();
    void IncrementWave();
    void QuitSurvival(bool playerDied);
    void TriggerDelayedSpawn();
    void ScriptQuit();
    bool PlayerCheated();
    void GiveReward(bool playerDied);
}
