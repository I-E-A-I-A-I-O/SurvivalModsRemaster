#pragma once

#include "SpawnData.hpp"
#include "Screen.hpp"
#include "Calculations.hpp"
#include "Initialization.hpp"
#include "natives.h"
#include "enums.h"
#include "json.hpp"
#include <fstream>
#include <vector>
#include <string>

namespace SURVIVAL
{
	struct SpawnerData
	{
		static Vector3 location;
		static std::vector<SpawnData> pickups;
        static std::vector<Hash> weakWeapons;
        static std::vector<Hash> medWeapons;
        static std::vector<Hash> strongWeapons;
        static bool isXmas;
        static bool isHalloween;
        static bool hasJuggernaut;
        static bool hasJesus;
        static bool hasDogs;
        static bool hasVehicles;
        static bool hasAircraft;
        static bool hasSuicidal;
        static bool hasBoats;
	};

    void LoadSurvival(const std::string& survivalID);
    Ped SpawnEnemy(int wave, bool canSpawnJesus);
    Ped SpawnDog();
    Ped SpawnJuggernaut();
    Vehicle SpawnVehicle(bool boat);
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
