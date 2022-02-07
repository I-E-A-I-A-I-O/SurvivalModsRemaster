#include "pch.h"
#include "Blip.hpp"

void BLIPS::SetBlipName(Blip blip, const char* name)
{
	HUD::BEGIN_TEXT_COMMAND_SET_BLIP_NAME("STRING");
    HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(name);
	HUD::END_TEXT_COMMAND_SET_BLIP_NAME(blip);
}

Blip BLIPS::CreateForMissionTriggerPed(Vector3 pos, const char* blipName)
{
	Blip blipHandle = HUD::ADD_BLIP_FOR_COORD(pos.x, pos.y, pos.z);
	HUD::SET_BLIP_SPRITE(blipHandle, eBlipSprite::BlipSpriteGTAOSurvival);
	HUD::SET_BLIP_COLOUR(blipHandle, eBlipColor::BlipColorRed);
	SetBlipName(blipHandle, blipName);
	HUD::SET_BLIP_AS_SHORT_RANGE(blipHandle, true);
	return blipHandle;
}

Blip BLIPS::Create(float x, float y, float z, int sprite, eBlipColor color, const char *name) {
    Blip blipHandle = HUD::ADD_BLIP_FOR_COORD(x, y, z);
    HUD::SET_BLIP_SPRITE(blipHandle, sprite);
    HUD::SET_BLIP_COLOUR(blipHandle, color);
    SetBlipName(blipHandle, name);
    HUD::SET_BLIP_AS_SHORT_RANGE(blipHandle, true);
    return blipHandle;
}

void BLIPS::CreateForEnemyPed(Ped ped)
{
	Blip blipHandle = HUD::ADD_BLIP_FOR_ENTITY(ped);

	HUD::SET_BLIP_SCALE(blipHandle, 0.7f);
	HUD::SET_BLIP_SPRITE(blipHandle, eBlipSprite::BlipSpriteEnemy);
	HUD::SET_BLIP_COLOUR(blipHandle, eBlipColor::BlipColorRed);
	SetBlipName(blipHandle, "Enemy");
	//HUD::SET_BLIP_AS_SHORT_RANGE(blipHandle, true);
}

void BLIPS::CreateForEnemyPed(Ped ped, int sprite, const char* name)
{
	Blip blipHandle = HUD::ADD_BLIP_FOR_ENTITY(ped);
	HUD::SET_BLIP_SPRITE(blipHandle, sprite);
	HUD::SET_BLIP_SCALE(blipHandle, 0.8f);
	HUD::SET_BLIP_COLOUR(blipHandle, eBlipColor::BlipColorRed);
	SetBlipName(blipHandle, name);
	//HUD::SET_BLIP_AS_SHORT_RANGE(blipHandle, true);
}

void BLIPS::CreateForEnemyVehicle(Vehicle vehicle)
{
	Blip blipHandle = HUD::ADD_BLIP_FOR_ENTITY(vehicle);
	Hash model = ENTITY::GET_ENTITY_MODEL(vehicle);

	if (VEHICLE::IS_THIS_MODEL_A_CAR(model))
	{
		HUD::SET_BLIP_SPRITE(blipHandle, eBlipSprite::BlipSpritePersonalVehicleCar);
	}
	else if (VEHICLE::IS_THIS_MODEL_A_BIKE(model))
	{
		HUD::SET_BLIP_SPRITE(blipHandle, eBlipSprite::BlipSpritePersonalVehicleBike);
	}
	else if (VEHICLE::IS_THIS_MODEL_A_HELI(model))
	{
		HUD::SET_BLIP_SPRITE(blipHandle, eBlipSprite::BlipSpriteHelicopterAnimated);
	}
	else if (VEHICLE::IS_THIS_MODEL_A_PLANE(model))
	{
		HUD::SET_BLIP_SPRITE(blipHandle, eBlipSprite::BlipSpritePlane);
	} else if (VEHICLE::IS_THIS_MODEL_A_BOAT(model))
    {
        HUD::SET_BLIP_SPRITE(blipHandle, eBlipSprite::BlipSpriteBoat);
    }

	HUD::SET_BLIP_SCALE(blipHandle, 0.8f);
	HUD::SET_BLIP_COLOUR(blipHandle, eBlipColor::BlipColorRed);
	SetBlipName(blipHandle, "Enemy vehicle");
	//HUD::SET_BLIP_AS_SHORT_RANGE(blipHandle, true);
}

Blip BLIPS::GetBlipForEntity(Entity entity)
{
	return HUD::GET_BLIP_FROM_ENTITY(entity);
}

void BLIPS::DeleteBlipForEntity(Entity entity)
{
	Blip blip = GetBlipForEntity(entity);
	HUD::REMOVE_BLIP(&blip);
}
