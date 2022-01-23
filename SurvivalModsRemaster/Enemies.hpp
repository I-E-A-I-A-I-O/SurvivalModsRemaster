#pragma once

#include "natives.h"
#include "Calculations.hpp"
#include "Jesus.hpp"
#include "Screen.hpp"
#include "SurvivalSpawner.hpp"
#include "Survival.hpp"
#include "Script.hpp"
#include "Music.hpp"
#include "Intermission.hpp"
#include "MissionTimers.hpp"
#include "Blip.hpp"
#include <vector>


namespace ENEMIES
{
	struct EnemiesData
	{
		static int currentAircraft;
		static int currentVehicles;
		static bool canSpawnMore;
		static bool limitReached;
		static int currentWaveSize;
		static int currentDogCount;
		static int kills;
	};

    struct Suicidal
    {
        Ped ped = 0;
        bool timer = false;
        bool exploded = false;
        Vector3 deadCoords{};
        int timeOfDeath = 0;
    };

    void Process();
    void RemoveUnusedVehicles();
	void ResetCounters();
	void ClearVectors();
}
