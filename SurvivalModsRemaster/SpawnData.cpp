#include "pch.h"
#include "SpawnData.hpp"
#include "Screen.hpp"

SpawnData::SpawnData(std::string m, float x, float y, float z, float h, std::string task)
{
	modelName = m;
	Vector3 vector = Vector3();
	vector.x = x;
	vector.y = y;
	vector.z = z;
	position = vector;
	heading = h;
	taskName = task;
}

SpawnData::SpawnData(ePickupType pT, float x, float y, float z, std::string m, int v, int sprite, std::string pickupName)
{
	pickupType = pT;
	Vector3 vector = Vector3();
	vector.x = x;
	vector.y = y;
	vector.z = z;
	position = vector;
	modelName = m;
	value = v;
	pickupSprite = sprite;
	stringName = pickupName;
}

SpawnData::SpawnData(std::string n)
{
	timerActive = false;
	stringName = n;
}

SpawnData::SpawnData(std::string m, bool mp, bool gender)
{
	modelName = m;
	isMp = mp;
	isMale = gender;
}

void SpawnData::StarTimer()
{
	timerActive = true;
}
