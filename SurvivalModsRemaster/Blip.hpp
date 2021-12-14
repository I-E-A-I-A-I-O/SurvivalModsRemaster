#pragma once
#include "natives.h"
#include "enums.h"

namespace BLIPS
{
	void CreateForMissionTriggerPed(Ped ped, const char* blipName);
	void CreateForEnemyPed(Ped ped);
	void CreateForEnemyVehicle(Vehicle vehicle);
	Blip GetBlipForEntity(Entity entity);
	void DeleteBlipForEntity(Entity entity);
	void CreateForEnemyPed(Ped ped, int sprite, const char* name);
	void SetBlipName(Blip blip, const char* name);
}
