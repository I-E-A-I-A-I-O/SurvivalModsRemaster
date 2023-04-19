#include "pch.h"
#include "SurvivalSpawner.hpp"

Vector3 SURVIVAL::SpawnerData::location;
std::vector<Vector3> enemySpawnpoints;
std::vector<Vector3> aircraftSpawnpoints;
std::vector<Vector3> vehicleSpawnpoints;
std::vector<Vector3> boatSpawnpoints;
std::vector<SpawnData> pedGroup1;
std::vector<SpawnData> pedGroup2;
std::vector<SpawnData> pedGroup3;
std::vector<std::string> vehicleGroup1;
std::vector<std::string> vehicleGroup2;
std::vector<std::string> vehicleGroup3;
std::vector<std::string> aircraftGroup1;
std::vector<std::string> aircraftGroup2;
std::vector<std::string> aircraftGroup3;
std::vector<std::string> boatGroup1;
std::vector<std::string> boatGroup2;
std::vector<std::string> boatGroup3;
std::vector<Hash> SURVIVAL::SpawnerData::weakWeapons;
std::vector<Hash> SURVIVAL::SpawnerData::medWeapons;
std::vector<Hash> SURVIVAL::SpawnerData::strongWeapons;
std::vector<SpawnData> currentPedModels;
std::vector<std::string> currentAircraftModels;
std::vector<std::string> currentVehicleModels;
std::vector<std::string> currentBoatModels;
std::vector<SpawnData> SURVIVAL::SpawnerData::pickups;
bool SURVIVAL::SpawnerData::isXmas;
bool SURVIVAL::SpawnerData::isHalloween;
bool SURVIVAL::SpawnerData::hasJuggernaut;
bool SURVIVAL::SpawnerData::hasJesus;
bool SURVIVAL::SpawnerData::hasDogs;
bool SURVIVAL::SpawnerData::hasVehicles;
bool SURVIVAL::SpawnerData::hasAircraft;
bool SURVIVAL::SpawnerData::hasSuicidal;
bool SURVIVAL::SpawnerData::hasBoats;
std::vector<std::string> jugModels;
std::string dogModel;

ePickupType SURVIVAL::GetPickupType(const std::string& pickupModel)
{
	if (pickupModel == "prop_armour_pickup")
	{
		return ePickupType::PickupTypeArmour;
	}
	else if (pickupModel == "prop_ld_health_pack")
	{
		return ePickupType::PickupTypeHealth;
	}
	else if (pickupModel == "w_lr_rpg")
	{
		return ePickupType::PickupTypeWeaponRPG;
	}
	else if (pickupModel == "w_ar_specialcarbine")
	{
		return ePickupType::PickupTypeWeaponSpecialCarbine;
	}
	else if (pickupModel == "w_sg_assaultshotgun")
	{
		return ePickupType::PickupTypeWeaponAssaultShotgun;
	}
	else if (pickupModel == "w_ex_pe")
	{
		return ePickupType::PickupTypeWeaponStickyBomb;
	}
	else if (pickupModel == "w_ex_molotov")
	{
		return ePickupType::PickupTypeWeaponMolotov;
	}
	else if (pickupModel == "w_mg_minigun")
	{
		return ePickupType::PickupTypeWeaponMinigun;
	}
	else if (pickupModel == "w_sr_heavysniper")
	{
		return ePickupType::PickupTypeWeaponHeavySniper;
	}
	else if (pickupModel == "w_lr_grenadelauncher")
	{
		return ePickupType::PickupTypeWeaponGrenadeLauncher;
	}
	else if (pickupModel == "w_ar_assaultrifle")
	{
		return ePickupType::PickupTypeWeaponAssaultRifle;
	}
	else if (pickupModel == "w_sg_sawnoff")
	{
		return ePickupType::PickupTypeWeaponSawnoffShotgun;
	}
	else if (pickupModel == "w_mg_combatmg")
	{
		return ePickupType::PickupTypeWeaponCombatMG;
	}
	else if (pickupModel == "w_sb_microsmg")
	{
		return ePickupType::PickupTypeWeaponMicroSMG;
	}
	else if (pickupModel == "w_pi_appistol")
	{
		return ePickupType::PickupTypeWeaponAPPistol;
	}
	else if (pickupModel == "w_ex_grenadefrag")
	{
		return ePickupType::PickupTypeWeaponGrenade;
	}
	else if (pickupModel == "w_pi_heavypistol")
	{
		return ePickupType::PickupTypeWeaponHeavyPistol;
	}
	else if (pickupModel == "w_ar_carbinerifle")
	{
		return ePickupType::PickupTypeWeaponCarbineRifle;
	}
}

int SURVIVAL::GetQuantity(const std::string& pickupModel)
{
	if (pickupModel == "prop_armour_pickup")
	{
		return 200;
	}
	else if (pickupModel == "prop_ld_health_pack")
	{
		return 100;
	}
	else if (pickupModel == "w_lr_rpg")
	{
		return 3;
	}
	else if (pickupModel == "w_ar_specialcarbine")
	{
		return 150;
	}
	else if (pickupModel == "w_sg_assaultshotgun")
	{
		return 25;
	}
	else if (pickupModel == "w_ex_pe")
	{
		return 3;
	}
	else if (pickupModel == "w_ex_molotov")
	{
		return 3;
	}
	else if (pickupModel == "w_mg_minigun")
	{
		return 350;
	}
	else if (pickupModel == "w_sr_heavysniper")
	{
		return 15;
	}
	else if (pickupModel == "w_lr_grenadelauncher")
	{
		return 5;
	}
	else if (pickupModel == "w_ar_assaultrifle")
	{
		return 200;
	}
	else if (pickupModel == "w_sg_sawnoff")
	{
		return 25;
	}
	else if (pickupModel == "w_mg_combatmg")
	{
		return 200;
	}
	else if (pickupModel == "w_sb_microsmg")
	{
		return 200;
	}
	else if (pickupModel == "w_pi_appistol")
	{
		return 120;
	}
	else if (pickupModel == "w_ex_grenadefrag")
	{
		return 3;
	}
	else if (pickupModel == "w_pi_heavypistol")
	{
		return 30;
	}
	else if (pickupModel == "w_ar_carbinerifle")
	{
		return 200;
	}
}

int SURVIVAL::GetSprite(const std::string& pickupModel)
{
	if (pickupModel == "prop_armour_pickup")
	{
		return 175;
	}
	else if (pickupModel == "prop_ld_health_pack")
	{
		return 153;
	}
	else if (pickupModel == "w_lr_rpg")
	{
		return 157;
	}
	else if (pickupModel == "w_ar_specialcarbine")
	{
		return 150;
	}
	else if (pickupModel == "w_sg_assaultshotgun")
	{
		return 158;
	}
	else if (pickupModel == "w_ex_pe")
	{
		return 152;
	}
	else if (pickupModel == "w_ex_molotov")
	{
		return 155;
	}
	else if (pickupModel == "w_mg_minigun")
	{
		return 173;
	}
	else if (pickupModel == "w_sr_heavysniper")
	{
		return 160;
	}
	else if (pickupModel == "w_lr_grenadelauncher")
	{
		return 157;
	}
	else if (pickupModel == "w_ar_assaultrifle")
	{
		return 150;
	}
	else if (pickupModel == "w_sg_sawnoff")
	{
		return 158;
	}
	else if (pickupModel == "w_mg_combatmg")
	{
		return 159;
	}
	else if (pickupModel == "w_sb_microsmg")
	{
		return 159;
	}
	else if (pickupModel == "w_pi_appistol")
	{
		return 156;
	}
	else if (pickupModel == "w_ex_grenadefrag")
	{
		return 152;
	}
	else if (pickupModel == "w_pi_heavypistol")
	{
		return 156;
	}
	else if (pickupModel == "w_ar_carbinerifle")
	{
		return 150;
	}
}

std::string SURVIVAL::GetPickupName(const std::string& pickupModel)
{
	if (pickupModel == "prop_armour_pickup")
	{
		return "Armor";
	}
	else if (pickupModel == "prop_ld_health_pack")
	{
		return "Health";
	}
	else if (pickupModel == "w_lr_rpg")
	{
		return "RPG";
	}
	else if (pickupModel == "w_ar_specialcarbine")
	{
		return "Special Carbine";
	}
	else if (pickupModel == "w_sg_assaultshotgun")
	{
		return "Assault Shotgun";
	}
	else if (pickupModel == "w_ex_pe")
	{
		return "Sticky Bomb";
	}
	else if (pickupModel == "w_ex_molotov")
	{
		return "Molotov";
	}
	else if (pickupModel == "w_mg_minigun")
	{
		return "Minigun";
	}
	else if (pickupModel == "w_sr_heavysniper")
	{
		return "Heavy Sniper";
	}
	else if (pickupModel == "w_lr_grenadelauncher")
	{
		return "Grenade Launcher";
	}
	else if (pickupModel == "w_ar_assaultrifle")
	{
		return "Assault Rifle";
	}
	else if (pickupModel == "w_sg_sawnoff")
	{
		return "Sawn-Off Shotgun";
	}
	else if (pickupModel == "w_mg_combatmg")
	{
		return "Combat MG";
	}
	else if (pickupModel == "w_sb_microsmg")
	{
		return "Micro SMG";
	}
	else if (pickupModel == "w_pi_appistol")
	{
		return "AP Pistol";
	}
	else if (pickupModel == "w_ex_grenadefrag")
	{
		return "Grenade";
	}
	else if (pickupModel == "w_pi_heavypistol")
	{
		return "Heavy Pistol";
	}
	else if (pickupModel == "w_ar_carbinerifle")
	{
		return "Carbine Rifle";
	}
}

void SURVIVAL::ClearVectors()
{
	enemySpawnpoints.clear();
	aircraftSpawnpoints.clear();
	vehicleSpawnpoints.clear();
    boatSpawnpoints.clear();
    boatGroup1.clear();
    boatGroup2.clear();
    boatGroup3.clear();
	SpawnerData::pickups.clear();
	pedGroup1.clear();
	pedGroup2.clear();
	pedGroup3.clear();
    vehicleGroup1.clear();
    vehicleGroup2.clear();
    vehicleGroup3.clear();
	SpawnerData::weakWeapons.clear();
	SpawnerData::medWeapons.clear();
	SpawnerData::strongWeapons.clear();
	jugModels.clear();
	dogModel.clear();
}

void SURVIVAL::LoadSurvival(const std::string& survivalID)
{
	try
	{
		ClearVectors();
		char fileName[150];
		strcpy_s(fileName, "SurvivalsData\\Survivals\\");
		strcat_s(fileName, survivalID.c_str());
		strcat_s(fileName, ".json");
		nlohmann::json js;
		std::ifstream i(fileName);
		i >> js;
		i.close();

        SpawnerData::isXmas = js["Flags"]["xmas"];
        SpawnerData::isHalloween = js["Flags"]["halloween"];
        SpawnerData::hasJuggernaut = js["Flags"]["juggernaut"];
        SpawnerData::hasDogs = js["Flags"]["dogs"];
        SpawnerData::hasJesus = js["Flags"]["jesus"];
        SpawnerData::hasAircraft = js["Flags"]["aircraft"];
        SpawnerData::hasVehicles = js["Flags"]["vehicles"];
        SpawnerData::hasSuicidal = js["Flags"]["bombers"];
        SpawnerData::hasBoats = js["Flags"]["boats"];
		std::vector<double> locationPoints = js["Location"];
		std::vector<double> pedSpawnpointsX = js["Spawnpoints"]["peds"]["x"];
		std::vector<double> pedSpawnpointsY = js["Spawnpoints"]["peds"]["y"];
		std::vector<double> pedSpawnpointsZ = js["Spawnpoints"]["peds"]["z"];
		std::vector<double> pickupSpawnpointsX = js["Spawnpoints"]["pickups"]["x"];
		std::vector<double> pickupSpawnpointsY = js["Spawnpoints"]["pickups"]["y"];
		std::vector<double> pickupSpawnpointsZ = js["Spawnpoints"]["pickups"]["z"];
		std::vector<std::string> pedsGroup1 = js["Models"]["peds"]["group1"];
		std::vector<std::string> pedsGroup2 = js["Models"]["peds"]["group2"];
		std::vector<std::string> pedsGroup3 = js["Models"]["peds"]["group3"];
		std::vector<std::string> weapons1 = js["Weapons"]["weak"];
		std::vector<std::string> weapons2 = js["Weapons"]["medium"];
		std::vector<std::string> weapons3 = js["Weapons"]["strong"];
		std::vector<std::string> pickupModels = js["Models"]["pickups"];

		SpawnerData::location.x = locationPoints.at(0);
		SpawnerData::location.y = locationPoints.at(1);
		SpawnerData::location.z = locationPoints.at(2);

        if (SpawnerData::hasDogs)
        {
            dogModel = js["Models"]["dog"];
        }

        if (SpawnerData::hasJuggernaut)
        {
            std::vector<std::string> jModels = js["Models"]["juggernaut"];

            for (auto & jModel : jModels)
            {
                jugModels.push_back(jModel);
            }
        }

        if (SpawnerData::hasAircraft)
        {
            std::vector<double> airSpawnpointsX = js["Spawnpoints"]["aircraft"]["x"];
            std::vector<double> airSpawnpointsY = js["Spawnpoints"]["aircraft"]["y"];
            std::vector<double> airSpawnpointsZ = js["Spawnpoints"]["aircraft"]["z"];
            std::vector<std::string> airGroup1 = js["Models"]["aircraft"]["group1"];
            std::vector<std::string> airGroup2 = js["Models"]["aircraft"]["group2"];
            std::vector<std::string> airGroup3 = js["Models"]["aircraft"]["group3"];

            for (size_t n = 0; n < airSpawnpointsX.size(); n++)
            {
                Vector3 spawnpoint = Vector3();
                spawnpoint.x = airSpawnpointsX.at(n);
                spawnpoint.y = airSpawnpointsY.at(n);
                spawnpoint.z = airSpawnpointsZ.at(n);
                aircraftSpawnpoints.push_back(spawnpoint);
            }

            aircraftGroup1 = airGroup1;
            aircraftGroup2 = airGroup2;
            aircraftGroup3 = airGroup3;
        }

        if (SpawnerData::hasVehicles)
        {
            std::vector<double> vehiclesSpawnpointsX = js["Spawnpoints"]["vehicles"]["x"];
            std::vector<double> vehiclesSpawnpointsY = js["Spawnpoints"]["vehicles"]["y"];
            std::vector<double> vehiclesSpawnpointsZ = js["Spawnpoints"]["vehicles"]["z"];
            std::vector<std::string> vehiclesGroup1 = js["Models"]["vehicles"]["group1"];
            std::vector<std::string> vehiclesGroup2 = js["Models"]["vehicles"]["group2"];
            std::vector<std::string> vehiclesGroup3 = js["Models"]["vehicles"]["group3"];

            for (size_t n = 0; n < vehiclesSpawnpointsX.size(); n++)
            {
                Vector3 spawnpoint = Vector3();
                spawnpoint.x = vehiclesSpawnpointsX.at(n);
                spawnpoint.y = vehiclesSpawnpointsY.at(n);
                spawnpoint.z = vehiclesSpawnpointsZ.at(n);
                vehicleSpawnpoints.push_back(spawnpoint);
            }

            vehicleGroup1 = vehiclesGroup1;
            vehicleGroup2 = vehiclesGroup2;
            vehicleGroup3 = vehiclesGroup3;
        }

        if (SpawnerData::hasBoats)
        {
            std::vector<double> boatSpawnpointsX = js["Spawnpoints"]["boats"]["x"];
            std::vector<double> boatSpawnpointsY = js["Spawnpoints"]["boats"]["y"];
            std::vector<double> boatSpawnpointsZ = js["Spawnpoints"]["boats"]["z"];
            std::vector<std::string> boatsGroup1 = js["Models"]["boats"]["group1"];
            std::vector<std::string> boatsGroup2 = js["Models"]["boats"]["group2"];
            std::vector<std::string> boatsGroup3 = js["Models"]["boats"]["group3"];

            for (size_t n = 0; n < boatSpawnpointsX.size(); n++)
            {
                Vector3 spawnpoint = Vector3();
                spawnpoint.x = boatSpawnpointsX.at(n);
                spawnpoint.y = boatSpawnpointsY.at(n);
                spawnpoint.z = boatSpawnpointsZ.at(n);
                boatSpawnpoints.push_back(spawnpoint);
            }

            boatGroup1 = boatsGroup1;
            boatGroup2 = boatsGroup2;
            boatGroup3 = boatsGroup3;
        }
		
		for (auto & item : weapons1)
		{
			Hash weaponHash = MISC::GET_HASH_KEY(item.c_str());
			SpawnerData::weakWeapons.push_back(weaponHash);
		}

		for (auto & item : weapons2)
		{
			Hash weaponHash = MISC::GET_HASH_KEY(item.c_str());
			SpawnerData::medWeapons.push_back(weaponHash);
		}

		for (auto & item : weapons3)
		{
			Hash weaponHash = MISC::GET_HASH_KEY(item.c_str());
			SpawnerData::strongWeapons.push_back(weaponHash);
		}

		for (size_t item = 0; item < pedSpawnpointsX.size(); item++)
		{
			Vector3 spawnpoint = Vector3();
			spawnpoint.z = pedSpawnpointsZ.at(item);
			spawnpoint.x = pedSpawnpointsX.at(item);
			spawnpoint.y = pedSpawnpointsY.at(item);
			enemySpawnpoints.push_back(spawnpoint);
		}

		for (size_t n = 0; n < pickupModels.size(); n++)
		{
			std::string model = pickupModels.at(n);
			std::string pickNam = GetPickupName(model);
			ePickupType pickType = GetPickupType(model);
			int pickVal = GetQuantity(model);
			int pickSpr = GetSprite(model);

			SpawnData pickData = SpawnData(pickType, pickupSpawnpointsX.at(n), pickupSpawnpointsY.at(n), pickupSpawnpointsZ.at(n), model, pickVal, pickSpr, pickNam);
			SpawnerData::pickups.push_back(pickData);
		}

		for (const auto& model : pedsGroup1)
		{
            SpawnData data = SpawnData(model, model.find("CUSTOM_") != std::string::npos, model.find("_M") != std::string::npos);
			pedGroup1.push_back(data);
		}

		for (const auto& model : pedsGroup2)
		{
            SpawnData data = SpawnData(model, model.find("CUSTOM_") != std::string::npos, model.find("_M") != std::string::npos);
			pedGroup2.push_back(data);
		}

		for (const auto& model : pedsGroup3)
		{
            SpawnData data = SpawnData(model, model.find("CUSTOM_") != std::string::npos, model.find("_M") != std::string::npos);
			pedGroup3.push_back(data);
		}
	}
	catch (std::exception& e)
	{
		SCREEN::ShowNotification(e.what());
	}
}

void SURVIVAL::SetComponentVariation(Ped ped, int componentId, int drawableId, int textureId)
{
	if (PED::IS_PED_COMPONENT_VARIATION_VALID(ped, componentId, drawableId, textureId))
	{
		PED::SET_PED_COMPONENT_VARIATION(ped, componentId, drawableId, textureId, 0);
	}
}

Vector3 safeSpawnpoint() {
    Vector3 playerCoords = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);

    if (enemySpawnpoints.size() > 1) {
        Vector3 coords;

        do {
            size_t index = CALC::RanInt(enemySpawnpoints.size() - (size_t)1, (size_t)0);
            coords = enemySpawnpoints.at(index);
        } while(CALC::IsInRange_2(coords, playerCoords, 20.0f));

        return coords;
    }

    return enemySpawnpoints.at(0);
}

Ped SURVIVAL::SpawnFreemodeCustom(const std::string& outfit, bool isMale, bool inVehicle, Vehicle vehicle, int seat)
{
	Hash model;

	if (isMale)
	{
		model = MISC::GET_HASH_KEY("MP_M_Freemode_01");
	}
	else
	{
		model = MISC::GET_HASH_KEY("MP_F_Freemode_01");
	}

	STREAMING::REQUEST_MODEL(model);

	while (!STREAMING::HAS_MODEL_LOADED(model))
	{
		WAIT(250);
	}

	Ped ped;

	if (!inVehicle)
	{
        Vector3 spawnpoint = safeSpawnpoint();
		ped = PED::CREATE_PED(0, model, spawnpoint.x, spawnpoint.y, spawnpoint.z, 0, false, true);
	}
	else
	{
		ped = PED::CREATE_PED_INSIDE_VEHICLE(vehicle, 0, model, seat, false, true);
	}

	STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);

	if (outfit.find("_JUGGERNAUT_") != std::string::npos && isMale)
	{
		SetComponentVariation(ped, 1, 91, 0);
		SetComponentVariation(ped, 4, 84, 0);
		SetComponentVariation(ped, 6, 3, 0);
		SetComponentVariation(ped, 8, 97, 0);
		SetComponentVariation(ped, 11, 186, 0);
	}
	else if (outfit.find("_JUGGERNAUT_") != std::string::npos && !isMale)
	{
		SetComponentVariation(ped, 1, 91, 0);
		SetComponentVariation(ped, 4, 86, 0);
		SetComponentVariation(ped, 6, 4, 0);
		SetComponentVariation(ped, 8, 97, 0);
		SetComponentVariation(ped, 11, 188, 0);
	}
	else if (outfit.find("_SANTA_") != std::string::npos)
	{
		SetComponentVariation(ped, 1, 8, 0);
		SetComponentVariation(ped, 11, 116, 0);
		SetComponentVariation(ped, 4, 19, 0);
		SetComponentVariation(ped, 8, 14, 0);
		SetComponentVariation(ped, 6, 10, 0);
	}
	else if (outfit.find("_BEAST_") != std::string::npos)
	{
		SetComponentVariation(ped, 1, 59, 0);
		SetComponentVariation(ped, 11, 79, 0);
		SetComponentVariation(ped, 6, 1, 0);
		SetComponentVariation(ped, 3, 97, 0);
	}
	else if (outfit.find("_FROSTY_") != std::string::npos)
	{
		SetComponentVariation(ped, 1, 10, 0);
		SetComponentVariation(ped, 11, 69, 1);
		SetComponentVariation(ped, 8, 20, 0);
		SetComponentVariation(ped, 4, 23, 0);
		SetComponentVariation(ped, 6, 28, 0);
		SetComponentVariation(ped, 3, 75, 0);
	}
	else if (outfit.find("_ELF_") != std::string::npos)
	{
		SetComponentVariation(ped, 1, 34, 0);
		SetComponentVariation(ped, 11, 18, 1);
		SetComponentVariation(ped, 4, 19, 1);
		SetComponentVariation(ped, 6, 17, 0);
		SetComponentVariation(ped, 8, 20, 0);
	}
	else if (outfit.find("_MSSANTA_") != std::string::npos)
	{
		SetComponentVariation(ped, 1, 88, 0);
		SetComponentVariation(ped, 6, 13, 8);
		SetComponentVariation(ped, 4, 37, 4);
		SetComponentVariation(ped, 3, 3, 0);
		SetComponentVariation(ped, 8, 51, 0);
		SetComponentVariation(ped, 11, 108, 0);
	}
	else if (outfit.find("_RUDOLPH_") != std::string::npos)
	{
		SetComponentVariation(ped, 1, 9, 0);
		SetComponentVariation(ped, 11, 18, 3);
		SetComponentVariation(ped, 8, 15, 0);
		SetComponentVariation(ped, 4, 8, 0);
		SetComponentVariation(ped, 6, 27, 0);
	}
	else if (outfit.find("_HSANTA_") != std::string::npos)
	{
		SetComponentVariation(ped, 1, 76, 0);
		SetComponentVariation(ped, 11, 116, 0);
		SetComponentVariation(ped, 4, 19, 0);
		SetComponentVariation(ped, 8, 14, 0);
		SetComponentVariation(ped, 6, 10, 0);
	}
	else if (outfit.find("_PENGUIN_") != std::string::npos)
	{
		SetComponentVariation(ped, 1, 31, 0);
		SetComponentVariation(ped, 11, 20, 0);
		SetComponentVariation(ped, 8, 26, 0);
		SetComponentVariation(ped, 4, 20, 0);
		SetComponentVariation(ped, 6, 28, 0);
		SetComponentVariation(ped, 3, 82, 0);
	}
	else if (outfit.find("_HMSSANTA_") != std::string::npos)
	{
		SetComponentVariation(ped, 1, 86, 0);
		SetComponentVariation(ped, 6, 13, 8);
		SetComponentVariation(ped, 4, 37, 4);
		SetComponentVariation(ped, 3, 3, 0);
		SetComponentVariation(ped, 8, 51, 0);
		SetComponentVariation(ped, 11, 108, 0);
	}
	else if (outfit.find("_HELF_") != std::string::npos)
	{
		SetComponentVariation(ped, 1, 87, 0);
		SetComponentVariation(ped, 11, 18, 1);
		SetComponentVariation(ped, 4, 19, 1);
		SetComponentVariation(ped, 6, 17, 0);
		SetComponentVariation(ped, 8, 20, 0);
	}
	else if (outfit.find("_SNOWBIGFOOT_") != std::string::npos)
	{
		SetComponentVariation(ped, 1, 84, 0);
		SetComponentVariation(ped, 3, 98, 2);
		SetComponentVariation(ped, 4, 20, 0);
		SetComponentVariation(ped, 6, 28, 0);
		SetComponentVariation(ped, 8, 4, 0);
		SetComponentVariation(ped, 11, 20, 0);
	}

	return ped;
}

Ped SURVIVAL::SpawnJuggernaut()
{
	int index;

	if (SpawnerData::isHalloween)
	{
		index = 1;
	}
	else if (SpawnerData::isXmas)
	{
		index = 2;
	}
	else
	{
		index = 0;
	}

	std::string name = jugModels.at(index);

	if (name.find("CUSTOM_") != std::string::npos)
	{
		return SpawnFreemodeCustom(name, name.find("_M") != std::string::npos);
	}
	else
	{
		Hash model = INIT::LoadModel(name.c_str());
		Ped ped;
		Vector3 spawnpoint = safeSpawnpoint();
		ped = PED::CREATE_PED(0, model, spawnpoint.x, spawnpoint.y, spawnpoint.z, 0, false, true);
		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);

		return ped;
	}
}

Ped SURVIVAL::SpawnDog()
{
    Vector3 spawnpoint = safeSpawnpoint();
	Hash model = INIT::LoadModel(dogModel.c_str());
	Ped ped = PED::CREATE_PED(0, model, spawnpoint.x, spawnpoint.y, spawnpoint.z, 0, false, true);
	INIT::UnloadModel(model);

	return ped;
}

Ped SURVIVAL::SpawnEnemy(int wave, bool canSpawnJesus)
{
	if (SpawnerData::hasJesus && canSpawnJesus && (wave >= 7 || SURVIVAL::SurvivalData::hardcore))
	{
		Hash model = 0xCE2CB751;
		size_t index = CALC::RanInt(enemySpawnpoints.size() - (size_t)1, (size_t)0);
		Vector3 spawnpoint = enemySpawnpoints.at(index);
		INIT::LoadModel(model);
		Ped ped = PED::CREATE_PED(0, model, spawnpoint.x, spawnpoint.y, spawnpoint.z, 0, false, true);
		INIT::UnloadModel(model);

		return ped;
	}
	else
	{
		size_t index = CALC::RanInt(currentPedModels.size() - (size_t)1, (size_t)0);
		SpawnData data = currentPedModels.at(index);

		if (data.isMp)
			return SpawnFreemodeCustom(data.modelName, data.isMale);
		else
		{
			Hash model = MISC::GET_HASH_KEY(data.modelName.c_str());
            Vector3 spawnpoint = safeSpawnpoint();
			INIT::LoadModel(model);
			Ped ped = PED::CREATE_PED(0, model, spawnpoint.x, spawnpoint.y, spawnpoint.z, 0, false, true);
			INIT::UnloadModel(model);

			return ped;
		}
	}
}

Vehicle SURVIVAL::SpawnVehicle(bool boat)
{
    std::string modelName;
    Vector3 spawnpoint;

    if (boat)
    {
        size_t index = CALC::RanInt(currentBoatModels.size() - (size_t)1, (size_t)0);
        modelName = currentBoatModels.at(index);
        index = CALC::RanInt(boatSpawnpoints.size() - (size_t)1, (size_t)0);
        spawnpoint = boatSpawnpoints.at(index);
    }
    else
    {
        size_t index = CALC::RanInt(currentVehicleModels.size() - (size_t)1, (size_t)0);
        modelName = currentVehicleModels.at(index);
        index = CALC::RanInt(vehicleSpawnpoints.size() - (size_t)1, (size_t)0);
        spawnpoint = vehicleSpawnpoints.at(index);
    }

	Hash model = INIT::LoadModel(modelName.c_str());
	Vehicle vehicle = VEHICLE::CREATE_VEHICLE(model, spawnpoint.x, spawnpoint.y, spawnpoint.z, 0, false, true, false);
	INIT::UnloadModel(model);
	return vehicle;
}

Vehicle SURVIVAL::SpawnAircraft()
{
	size_t index = CALC::RanInt(currentAircraftModels.size() - (size_t)1, (size_t)0);
	std::string modelName = currentAircraftModels.at(index);
	index = CALC::RanInt(aircraftSpawnpoints.size() - (size_t)1, (size_t)0);
	Vector3 spawnPoint = aircraftSpawnpoints.at(index);
	Hash model = INIT::LoadModel(modelName.c_str());
	Vehicle vehicle = VEHICLE::CREATE_VEHICLE(model, spawnPoint.x, spawnPoint.y, spawnPoint.z, 0, false, true, false);
	INIT::UnloadModel(model);
	return vehicle;
}

std::vector<Ped> SURVIVAL::SpawnEnemiesInVehicle(Vehicle vehicle, int wave)
{
	int seats = VEHICLE::GET_VEHICLE_MAX_NUMBER_OF_PASSENGERS(vehicle);
	std::vector<Ped> peds;
	Ped ped;

	for (int i = -1; i < seats; i++)
	{
		size_t index = CALC::RanInt(currentPedModels.size() - (size_t)1, (size_t)0);
		SpawnData data = currentPedModels.at(index);

		if (data.isMp)
			ped = SpawnFreemodeCustom(data.modelName, data.isMale, true, vehicle, i);
		else
		{
			Hash hash = MISC::GET_HASH_KEY(data.modelName.c_str());
			INIT::LoadModel(hash);
			ped = PED::CREATE_PED_INSIDE_VEHICLE(vehicle, 0, hash, i, false, true);
			INIT::UnloadModel(hash);
		}
		peds.push_back(ped);
	}

	return peds;
}

void SURVIVAL::UpdateModels(int currentWave)
{
	if (SurvivalData::hardcore)
	{
		currentAircraftModels = aircraftGroup3;
		currentVehicleModels = vehicleGroup3;
		currentPedModels = pedGroup3;
        currentBoatModels = boatGroup3;
	}
	else
	{
		switch (currentWave)
		{
			case 1:
				currentPedModels = pedGroup1;
				break;
			case 3:
				currentVehicleModels = vehicleGroup1;
                currentBoatModels = boatGroup1;
				break;
			case 4:
				currentPedModels = pedGroup2;
				break;
			case 5:
				currentAircraftModels = aircraftGroup1;
				currentVehicleModels = vehicleGroup2;
                currentBoatModels = boatGroup2;
				break;
			case 6:
				currentAircraftModels = aircraftGroup2;
				break;
			case 7:
				currentVehicleModels = vehicleGroup3;
				currentPedModels = pedGroup3;
                currentBoatModels = boatGroup3;
				break;
			case 8:
				currentAircraftModels = aircraftGroup3;
				break;
			default:
				break;
		}
	}
}
