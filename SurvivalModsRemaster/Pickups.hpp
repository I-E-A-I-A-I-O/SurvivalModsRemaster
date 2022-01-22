#pragma once

#include "natives.h"
#include "SurvivalSpawner.hpp"
#include "Blip.hpp"
#include "Initialization.hpp"
#include <vector>

namespace PICKUPS
{
	struct PickupsData
	{
		static bool regenerating;
		static std::vector<Pickup> pickups;
	};

	void Initialize();
	Pickup SpawnPickup(const SpawnData& pickup);
	void Delete();
	void Regenerate();
	void Process();
}
