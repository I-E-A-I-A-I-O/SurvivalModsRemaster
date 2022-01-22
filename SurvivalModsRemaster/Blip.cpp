#include "pch.h"
#include "Blip.hpp"

void BLIPS::SetBlipName(Blip blip, const char* name)
{
	UI::BEGIN_TEXT_COMMAND_SET_BLIP_NAME((char*)"STRING");
	UI::_ADD_TEXT_COMPONENT_STRING((char*)name);
	UI::END_TEXT_COMMAND_SET_BLIP_NAME(blip);
}

Blip BLIPS::CreateForMissionTriggerPed(Vector3 pos, const char* blipName)
{
	Blip blipHandle = UI::ADD_BLIP_FOR_COORD(pos.x, pos.y, pos.z);
	UI::SET_BLIP_SPRITE(blipHandle, 630);
	UI::SET_BLIP_COLOUR(blipHandle, eBlipColor::BlipColorRed);
	SetBlipName(blipHandle, blipName);
	UI::SET_BLIP_AS_SHORT_RANGE(blipHandle, true);
	return blipHandle;
}

void BLIPS::CreateForEnemyPed(Ped ped)
{
	Blip blipHandle = UI::ADD_BLIP_FOR_ENTITY(ped);

	UI::SET_BLIP_SCALE(blipHandle, 0.7f);
	UI::SET_BLIP_SPRITE(blipHandle, eBlipSprite::BlipSpriteEnemy);
	UI::SET_BLIP_COLOUR(blipHandle, eBlipColor::BlipColorRed);
	SetBlipName(blipHandle, (char*)"Enemy");
	UI::SET_BLIP_AS_SHORT_RANGE(blipHandle, true);
}

void BLIPS::CreateForEnemyPed(Ped ped, int sprite, const char* name)
{
	Blip blipHandle = UI::ADD_BLIP_FOR_ENTITY(ped);
	UI::SET_BLIP_SPRITE(blipHandle, sprite);
	UI::SET_BLIP_SCALE(blipHandle, 0.8f);
	UI::SET_BLIP_COLOUR(blipHandle, eBlipColor::BlipColorRed);
	SetBlipName(blipHandle, name);
	UI::SET_BLIP_AS_SHORT_RANGE(blipHandle, true);
}

void BLIPS::CreateForEnemyVehicle(Vehicle vehicle)
{
	Blip blipHandle = UI::ADD_BLIP_FOR_ENTITY(vehicle);
	Hash model = ENTITY::GET_ENTITY_MODEL(vehicle);

	if (VEHICLE::IS_THIS_MODEL_A_CAR(model))
	{
		UI::SET_BLIP_SPRITE(blipHandle, eBlipSprite::BlipSpritePersonalVehicleCar);
	}
	else if (VEHICLE::IS_THIS_MODEL_A_BIKE(model))
	{
		UI::SET_BLIP_SPRITE(blipHandle, eBlipSprite::BlipSpritePersonalVehicleBike);
	}
	else if (VEHICLE::IS_THIS_MODEL_A_HELI(model))
	{
		UI::SET_BLIP_SPRITE(blipHandle, eBlipSprite::BlipSpriteHelicopterAnimated);
	}
	else if (VEHICLE::IS_THIS_MODEL_A_PLANE(model))
	{
		UI::SET_BLIP_SPRITE(blipHandle, eBlipSprite::BlipSpritePlane);
	}

	UI::SET_BLIP_SCALE(blipHandle, 0.8f);
	UI::SET_BLIP_COLOUR(blipHandle, eBlipColor::BlipColorRed);
	SetBlipName(blipHandle, (char*)"Enemy vehicle");
	UI::SET_BLIP_AS_SHORT_RANGE(blipHandle, true);
}

Blip BLIPS::GetBlipForEntity(Entity entity)
{
	return UI::GET_BLIP_FROM_ENTITY(entity);
}

void BLIPS::DeleteBlipForEntity(Entity entity)
{
	Blip blip = GetBlipForEntity(entity);
	UI::REMOVE_BLIP(&blip);
}
