#include "pch.h"
#include "Pickups.hpp"

bool PICKUPS::PickupsData::regenerating;
std::vector<Pickup> PICKUPS::PickupsData::pickups;

Pickup PICKUPS::SpawnPickup(const SpawnData& pickup)
{
	//Hash modelHash = INIT::LoadModel(pickup.modelName.c_str());
	Pickup pickupHandle = OBJECT::CREATE_PICKUP_ROTATE(pickup.pickupType,pickup.position.x, pickup.position.y, pickup.position.z, 0,0,0,
		8,pickup.value,2,true,0);
	//INIT::UnloadModel(modelHash);
	Blip blip = HUD::ADD_BLIP_FOR_PICKUP(pickupHandle);
	HUD::SET_BLIP_SPRITE(blip, pickup.pickupSprite);
	BLIPS::SetBlipName(blip, pickup.stringName.c_str());

	if (pickup.stringName == "Armor") {
		HUD::SET_BLIP_COLOUR(blip, 3);
	}
	else if (pickup.stringName == "Health")
	{
		HUD::SET_BLIP_COLOUR(blip, 2);
	}

	return pickupHandle;
}

void PICKUPS::Initialize()
{
	for (const SpawnData& pickup : SURVIVAL::SpawnerData::pickups)
	{
		Pickup handle = SpawnPickup(pickup);
		PickupsData::pickups.push_back(handle);
	}
}

void PICKUPS::Delete()
{
	for (Pickup pickup : PickupsData::pickups)
	{
		if (OBJECT::DOES_PICKUP_OBJECT_EXIST(pickup))
		{
			OBJECT::REMOVE_PICKUP(pickup);
		}
	}

	PickupsData::pickups.clear();
}

void PICKUPS::Regenerate()
{
	PickupsData::regenerating = true;
	Delete();
	Initialize();
	PickupsData::regenerating = false;
}

void PICKUPS::Process()
{
	if (PickupsData::regenerating)
	{
		return;
	}

	for (int i = 0; i < PickupsData::pickups.size(); i++)
	{
		if (!OBJECT::DOES_PICKUP_OBJECT_EXIST(PickupsData::pickups.at(i)))
		{
			OBJECT::REMOVE_PICKUP(PickupsData::pickups.at(i));
			PickupsData::pickups.erase(PickupsData::pickups.begin() + i);
			break;
		}
	}
}
