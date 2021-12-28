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
		static bool jugSpawned;
		static bool dogLimitReached;
		static int currentDogCount;
		static bool jesusSpawned;
		static int kills;
		static std::vector<Ped> footEnemies;
		static std::vector<Vehicle> enemyVehicles;
		static std::vector<Hash> alienWeapons;
		static std::vector<Ped> deadEnemies;
		static JESUS::Jesus enemyJesus;
		static Ped enemyJuggernaut;
	};

	void ResetCounters();
	void ClearVectors();
	void RemoveUnusedVehicles();
	void Process();
	void RemoveDeadEnemies();
	void ProcessJesus();
	std::vector<Hash> GetWeapons(Hash pedModel);
	void InitializeEnemyInAircraft(Ped ped, bool passenger);
	void InitializeEnemyInVehicle(Ped ped, bool passenger);
	void InitializeEnemy(Ped ped);
	void InitializeJuggernaut(Ped ped);
	void InitializeJesus(Ped ped);
	void InitializeAnimal(Ped ped);
	void InitializeRageEnemy(Ped ped);
	int GetKillTime(Ped ped);
}
