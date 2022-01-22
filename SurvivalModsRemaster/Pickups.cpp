#include "pch.h"
#include "Pickups.hpp"

bool PICKUPS::PickupsData::regenerating;
std::vector<Pickup> PICKUPS::PickupsData::pickups;

Pickup PICKUPS::SpawnPickup(const SpawnData& pickup)
{
	Hash modelHash = INIT::LoadModel(pickup.modelName.c_str());
	Pickup pickupHandle = OBJECT::CREATE_PICKUP(pickup.pickupType, pickup.position.x, pickup.position.y, pickup.position.z, 0, pickup.value, true, modelHash);
	INIT::UnloadModel(modelHash);
	Blip blip = UI::ADD_BLIP_FOR_PICKUP(pickupHandle);
	UI::SET_BLIP_SPRITE(blip, pickup.pickupSprite);
	BLIPS::SetBlipName(blip, pickup.stringName.c_str());
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
