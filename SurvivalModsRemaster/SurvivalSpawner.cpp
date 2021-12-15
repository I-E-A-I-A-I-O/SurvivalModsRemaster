#include "pch.h"
#include "SurvivalSpawner.hpp"

Vector3 SURVIVAL::SpawnerData::location;
std::vector<Vector3> SURVIVAL::SpawnerData::enemySpawnpoints;
std::vector<Vector3> SURVIVAL::SpawnerData::aircraftSpawnpoints;
std::vector<Vector3> SURVIVAL::SpawnerData::vehicleSpawnpoints;
std::vector<SpawnData> SURVIVAL::SpawnerData::pedsGroup1;
std::vector<SpawnData> SURVIVAL::SpawnerData::pedsGroup2;
std::vector<SpawnData> SURVIVAL::SpawnerData::pedsGroup3;
std::vector<std::string> SURVIVAL::SpawnerData::vehiclesGroup1;
std::vector<std::string> SURVIVAL::SpawnerData::vehiclesGroup2;
std::vector<std::string> SURVIVAL::SpawnerData::vehiclesGroup3;
std::vector<std::string> SURVIVAL::SpawnerData::aircraftGroup1;
std::vector<std::string> SURVIVAL::SpawnerData::aircraftGroup2;
std::vector<std::string> SURVIVAL::SpawnerData::aircraftGroup3;
std::vector<SpawnData> SURVIVAL::SpawnerData::currentPedModels;
std::vector<std::string> SURVIVAL::SpawnerData::currentAircraftModels;
std::vector<std::string> SURVIVAL::SpawnerData::currentVehicleModels;
std::vector<SpawnData> SURVIVAL::SpawnerData::pickups;
bool SURVIVAL::SpawnerData::isXmas;
bool SURVIVAL::SpawnerData::isHalloween;
bool SURVIVAL::SpawnerData::hasJuggernaut;
bool SURVIVAL::SpawnerData::hasJesus;
bool SURVIVAL::SpawnerData::hasDogs;

ePickupType SURVIVAL::GetPickupType(std::string pickupModel)
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

int SURVIVAL::GetQuantity(std::string pickupModel)
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

int SURVIVAL::GetSprite(std::string pickupModel)
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

std::string SURVIVAL::GetPickupName(std::string pickupModel)
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
	SpawnerData::enemySpawnpoints.clear();
	SpawnerData::aircraftSpawnpoints.clear();
	SpawnerData::vehicleSpawnpoints.clear();
	SpawnerData::pickups.clear();
	SpawnerData::pedsGroup1.clear();
	SpawnerData::pedsGroup2.clear();
	SpawnerData::pedsGroup3.clear();
}

void SURVIVAL::LoadSurvival(std::string survivalID)
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

		std::vector<double> locationPoints = js["Location"];
		std::vector<double> pedSpawnpointsX = js["Spawnpoints"]["peds"]["x"];
		std::vector<double> pedSpawnpointsY = js["Spawnpoints"]["peds"]["y"];
		std::vector<double> pedSpawnpointsZ = js["Spawnpoints"]["peds"]["z"];
		std::vector<double> vehicleSpawnpointsX = js["Spawnpoints"]["vehicles"]["x"];
		std::vector<double> vehicleSpawnpointsY = js["Spawnpoints"]["vehicles"]["y"];
		std::vector<double> vehicleSpawnpointsZ = js["Spawnpoints"]["vehicles"]["z"];
		std::vector<double> aircraftSpawnpointsX = js["Spawnpoints"]["aircraft"]["x"];
		std::vector<double> aircraftSpawnpointsY = js["Spawnpoints"]["aircraft"]["y"];
		std::vector<double> aircraftSpawnpointsZ = js["Spawnpoints"]["aircraft"]["z"];
		std::vector<double> pickupSpawnpointsX = js["Spawnpoints"]["pickups"]["x"];
		std::vector<double> pickupSpawnpointsY = js["Spawnpoints"]["pickups"]["y"];
		std::vector<double> pickupSpawnpointsZ = js["Spawnpoints"]["pickups"]["z"];
		std::vector<std::string> pedsGroup1 = js["Models"]["peds"]["group1"];
		std::vector<std::string> pedsGroup2 = js["Models"]["peds"]["group2"];
		std::vector<std::string> pedsGroup3 = js["Models"]["peds"]["group3"];
		std::vector<std::string> vehiclesGroup1 = js["Models"]["vehicles"]["group1"];
		std::vector<std::string> vehiclesGroup2 = js["Models"]["vehicles"]["group2"];
		std::vector<std::string> vehiclesGroup3 = js["Models"]["vehicles"]["group3"];
		std::vector<std::string> aircraftGroup1 = js["Models"]["aircraft"]["group1"];
		std::vector<std::string> aircraftGroup2 = js["Models"]["aircraft"]["group2"];
		std::vector<std::string> aircraftGroup3 = js["Models"]["aircraft"]["group3"];
		std::vector<std::string> pickupModels = js["Models"]["pickups"];
		SpawnerData::isXmas = js["Flags"]["xmas"];
		SpawnerData::isHalloween = js["Flags"]["halloween"];
		SpawnerData::hasJuggernaut = js["Flags"]["juggernaut"];
		SpawnerData::hasDogs = js["Flags"]["dogs"];
		SpawnerData::hasJesus = js["Flags"]["jesus"];

		SpawnerData::location.x = locationPoints.at(0);
		SpawnerData::location.y = locationPoints.at(1);
		SpawnerData::location.z = locationPoints.at(2);
		
		for (size_t i = 0; i < pedSpawnpointsX.size(); i++)
		{
			Vector3 spawnpoint = Vector3();
			spawnpoint.x = pedSpawnpointsX.at(i);
			spawnpoint.y = pedSpawnpointsY.at(i);
			spawnpoint.z = pedSpawnpointsZ.at(i);
			SpawnerData::enemySpawnpoints.push_back(spawnpoint);
		}

		for (size_t i = 0; i < vehicleSpawnpointsX.size(); i++)
		{
			Vector3 spawnpoint = Vector3();
			spawnpoint.x = vehicleSpawnpointsX.at(i);
			spawnpoint.y = vehicleSpawnpointsY.at(i);
			spawnpoint.z = vehicleSpawnpointsZ.at(i);
			SpawnerData::vehicleSpawnpoints.push_back(spawnpoint);
		}

		for (size_t i = 0; i < aircraftSpawnpointsX.size(); i++)
		{
			Vector3 spawnpoint = Vector3();
			spawnpoint.x = aircraftSpawnpointsX.at(i);
			spawnpoint.y = aircraftSpawnpointsY.at(i);
			spawnpoint.z = aircraftSpawnpointsZ.at(i);
			SpawnerData::aircraftSpawnpoints.push_back(spawnpoint);
		}

		for (size_t i = 0; i < pickupModels.size(); i++)
		{
			std::string model = pickupModels.at(i);
			std::string pickNam = GetPickupName(model);
			ePickupType pickType = GetPickupType(model);
			int pickVal = GetQuantity(model);
			int pickSpr = GetSprite(model);

			SpawnData pickData = SpawnData(pickType, pickupSpawnpointsX.at(i), pickupSpawnpointsY.at(i), pickupSpawnpointsZ.at(i), model, pickVal, pickSpr, pickNam);
			SpawnerData::pickups.push_back(pickData);
		}

		for (size_t i = 0; i < pedsGroup1.size(); i++)
		{
			std::string model = pedsGroup1.at(i);
			SpawnData data = SpawnData(model, model.find("CUSTOM_") != std::string::npos, model.find("_M") != std::string::npos);
			SpawnerData::pedsGroup1.push_back(data);
		}

		for (size_t i = 0; i < pedsGroup2.size(); i++)
		{
			std::string model = pedsGroup2.at(i);
			SpawnData data = SpawnData(model, model.find("CUSTOM_") != std::string::npos, model.find("_M") != std::string::npos);
			SpawnerData::pedsGroup2.push_back(data);
		}

		for (size_t i = 0; i < pedsGroup3.size(); i++)
		{
			std::string model = pedsGroup3.at(i);
			SpawnData data = SpawnData(model, model.find("CUSTOM_") != std::string::npos, model.find("_M") != std::string::npos);
			SpawnerData::pedsGroup3.push_back(data);
		}

		SpawnerData::vehiclesGroup1 = vehiclesGroup1;
		SpawnerData::vehiclesGroup2 = vehiclesGroup2;
		SpawnerData::vehiclesGroup3 = vehiclesGroup3;
		SpawnerData::aircraftGroup1 = aircraftGroup1;
		SpawnerData::aircraftGroup2 = aircraftGroup2;
		SpawnerData::aircraftGroup3 = aircraftGroup3;
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

Ped SURVIVAL::SpawnFreemodeCustom(std::string outfit, bool isMale, bool inVehicle, Vehicle vehicle, int seat)
{
	Hash model;

	if (isMale)
	{
		model = GAMEPLAY::GET_HASH_KEY((char*)"MP_M_Freemode_01");
	}
	else
	{
		model = GAMEPLAY::GET_HASH_KEY((char*)"MP_F_Freemode_01");
	}

	STREAMING::REQUEST_MODEL(model);

	while (!STREAMING::HAS_MODEL_LOADED(model))
	{
		WAIT(250);
	}

	Ped ped;

	if (!inVehicle)
	{
		size_t index = CALC::RanInt(SpawnerData::enemySpawnpoints.size() - (size_t)1, (size_t)0);
		Vector3 spawnpoint = SpawnerData::enemySpawnpoints.at(index);
		ped = PED::CREATE_PED(0, model, spawnpoint.x, spawnpoint.y, spawnpoint.z, 0, false, true);
	}
	else
	{
		ped = PED::CREATE_PED_INSIDE_VEHICLE(vehicle, 0, model, seat, false, true);
	}

	STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);

	if (outfit.find("_JUGGERNAUT_") != std::string::npos)
	{
		SetComponentVariation(ped, 1, 91, 0);
		SetComponentVariation(ped, 4, 84, 0);
		SetComponentVariation(ped, 6, 3, 0);
		SetComponentVariation(ped, 8, 97, 0);
		SetComponentVariation(ped, 11, 186, 0);
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
	if (SpawnerData::isHalloween)
	{
		return SpawnFreemodeCustom("CUSTOM_BEAST_M", true);
	}
	else if (SpawnerData::isXmas)
	{
		return SpawnFreemodeCustom("CUSTOM_SNOWBIGFOOT_M", true);
	}
	else
	{
		return SpawnFreemodeCustom("CUSTOM_JUGGERNAUT_M", true);
	}
}

Ped SURVIVAL::SpawnDog()
{
	Hash model = 0x9563221D;
	size_t index = CALC::RanInt(SpawnerData::enemySpawnpoints.size() - (size_t)1, (size_t)0);
	Vector3 spawnpoint = SpawnerData::enemySpawnpoints.at(index);
	INIT::LoadModel(model);
	Ped ped = PED::CREATE_PED(0, model, spawnpoint.x, spawnpoint.y, spawnpoint.z, 0, false, true);
	INIT::UnloadModel(model);

	return ped;
}

Ped SURVIVAL::SpawnEnemy(int wave, bool canSpawnJesus)
{
	if (SpawnerData::hasJesus && canSpawnJesus && (wave >= 7 || SURVIVAL::SurvivalData::hardcore))
	{
		Hash model = 0xCE2CB751;
		size_t index = CALC::RanInt(SpawnerData::enemySpawnpoints.size() - (size_t)1, (size_t)0);
		Vector3 spawnpoint = SpawnerData::enemySpawnpoints.at(index);
		INIT::LoadModel(model);
		Ped ped = PED::CREATE_PED(0, model, spawnpoint.x, spawnpoint.y, spawnpoint.z, 0, false, true);
		INIT::UnloadModel(model);

		return ped;
	}
	else
	{
		size_t index = CALC::RanInt(SpawnerData::currentPedModels.size() - (size_t)1, (size_t)0);
		SpawnData data = SpawnerData::currentPedModels.at(index);

		if (data.isMp)
			return SpawnFreemodeCustom(data.modelName, data.isMale);
		else
		{
			Hash model = GAMEPLAY::GET_HASH_KEY((char*)data.modelName.c_str());
			size_t index = CALC::RanInt(SpawnerData::enemySpawnpoints.size() - (size_t)1, (size_t)0);
			Vector3 spawnpoint = SpawnerData::enemySpawnpoints.at(index);
			INIT::LoadModel(model);
			Ped ped = PED::CREATE_PED(0, model, spawnpoint.x, spawnpoint.y, spawnpoint.z, 0, false, true);
			INIT::UnloadModel(model);

			return ped;
		}
	}
}

Vehicle SURVIVAL::SpawnVehicle()
{
	size_t index = CALC::RanInt(SpawnerData::currentVehicleModels.size() - (size_t)1, (size_t)0);
	std::string modelName = SpawnerData::currentVehicleModels.at(index);
	index = CALC::RanInt(SpawnerData::vehicleSpawnpoints.size() - (size_t)1, (size_t)0);
	Vector3 spawnPoint = SpawnerData::vehicleSpawnpoints.at(index);
	Hash model = INIT::LoadModel(modelName.c_str());
	Vehicle vehicle = VEHICLE::CREATE_VEHICLE(model, spawnPoint.x, spawnPoint.y, spawnPoint.z, 0, false, true);
	INIT::UnloadModel(model);
	return vehicle;
}

Vehicle SURVIVAL::SpawnAircraft()
{
	size_t index = CALC::RanInt(SpawnerData::currentAircraftModels.size() - (size_t)1, (size_t)0);
	std::string modelName = SpawnerData::currentAircraftModels.at(index);
	index = CALC::RanInt(SpawnerData::aircraftSpawnpoints.size() - (size_t)1, (size_t)0);
	Vector3 spawnPoint = SpawnerData::aircraftSpawnpoints.at(index);
	Hash model = INIT::LoadModel(modelName.c_str());
	Vehicle vehicle = VEHICLE::CREATE_VEHICLE(model, spawnPoint.x, spawnPoint.y, spawnPoint.z, 0, false, true);
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
		size_t index = CALC::RanInt(SpawnerData::currentPedModels.size() - (size_t)1, (size_t)0);
		SpawnData data = SpawnerData::currentPedModels.at(index);

		if (data.isMp)
			ped = SpawnFreemodeCustom(data.modelName, data.isMale, true, vehicle, i);
		else
		{
			Hash hash = GAMEPLAY::GET_HASH_KEY((char*)data.modelName.c_str());
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
		SpawnerData::currentAircraftModels = SpawnerData::aircraftGroup3;
		SpawnerData::currentVehicleModels = SpawnerData::vehiclesGroup3;
		SpawnerData::currentPedModels = SpawnerData::pedsGroup3;
	}
	else
	{
		switch (currentWave)
		{
			case 1:
				SpawnerData::currentPedModels = SpawnerData::pedsGroup1;
				break;
			case 3:
				SpawnerData::currentVehicleModels = SpawnerData::vehiclesGroup1;
				break;
			case 4:
				SpawnerData::currentPedModels = SpawnerData::pedsGroup2;
				break;
			case 5:
				SpawnerData::currentAircraftModels = SpawnerData::aircraftGroup1;
				SpawnerData::currentVehicleModels = SpawnerData::vehiclesGroup2;
				break;
			case 6:
				SpawnerData::currentAircraftModels = SpawnerData::aircraftGroup2;
				break;
			case 7:
				SpawnerData::currentVehicleModels = SpawnerData::vehiclesGroup3;
				SpawnerData::currentPedModels = SpawnerData::pedsGroup3;
				break;
			case 8:
				SpawnerData::currentAircraftModels = SpawnerData::aircraftGroup3;
				break;
			default:
				break;
		}
	}
}
