#pragma once

#include "types.h"
#include "enums.h"
#include <string>

class SpawnData
{
public:
	Vector3 position;
	float heading;
	std::string modelName;
	ePickupType pickupType;
	int value;
	std::string name;
	bool timerActive;
	Hash modelHash;
	std::string stringName;
	std::string taskName;
	int pickupSprite;
	bool isMp;
	bool isMale;

	SpawnData(std::string m, bool mp, bool gender);
	SpawnData(std::string m, float x, float y, float z, float h, std::string task);
	explicit SpawnData(std::string n);
	SpawnData(ePickupType pT, float x, float y, float z, std::string m, int v, int sprite, std::string pickupName);

	void StarTimer();
};
