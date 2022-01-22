#pragma once

#include "SpawnData.hpp"
#include "Screen.hpp"
#include "Calculations.hpp"
#include "Initialization.hpp"
#include "natives.h"
#include "enums.h"
#include "nlohmann/json.hpp"
#include <fstream>
#include <vector>
#include <string>

namespace SURVIVAL
{
	struct SpawnerData
	{
		static Vector3 location;
		static std::vector<SpawnData> pickups;
        static std::vector<Vector3> enemySpawnpoints;
        static std::vector<Vector3> aircraftSpawnpoints;
        static std::vector<Vector3> vehicleSpawnpoints;
        static std::vector<SpawnData> pedsGroup1;
        static std::vector<SpawnData> pedsGroup2;
        static std::vector<SpawnData> pedsGroup3;
        static std::vector<std::string> vehiclesGroup1;
        static std::vector<std::string> vehiclesGroup2;
        static std::vector<std::string> vehiclesGroup3;
        static std::vector<std::string> aircraftGroup1;
        static std::vector<std::string> aircraftGroup2;
        static std::vector<std::string> aircraftGroup3;
        static std::vector<Hash> weakWeapons;
        static std::vector<Hash> medWeapons;
        static std::vector<Hash> strongWeapons;
        static std::vector<SpawnData> currentPedModels;
        static std::vector<std::string> currentAircraftModels;
        static std::vector<std::string> currentVehicleModels;
        static bool isXmas;
        static bool isHalloween;
        static bool hasJuggernaut;
        static bool hasJesus;
        static bool hasDogs;
        static bool hasVehicles;
        static bool hasAircraft;
	};

    void LoadSurvival(const std::string& survivalID);
    Ped SpawnEnemy(int wave, bool canSpawnJesus);
    Ped SpawnDog();
    Ped SpawnJuggernaut();
    Vehicle SpawnVehicle();
    Vehicle SpawnAircraft();
    void UpdateModels(int curretnWave);
    std::vector<Ped> SpawnEnemiesInVehicle(Vehicle vehicle, int wave);
    Ped SpawnFreemodeCustom(const std::string& outfit, bool isMale, bool inVehicle = false, Vehicle vehicle = 0, int seat = -1);
    void SetComponentVariation(Ped ped, int componentId, int drawableId, int textureId);
    ePickupType GetPickupType(const std::string& pickupModel);
    int GetQuantity(const std::string& pickupModel);
    int GetSprite(const std::string& pickupModel);
    std::string GetPickupName(const std::string& pickupModel);
    void ClearVectors();
}
