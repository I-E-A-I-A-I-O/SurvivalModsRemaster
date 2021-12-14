#pragma once

#include "natives.h"
#include "Music.hpp"
#include "Enemies.hpp"
#include "MissionTimers.hpp"
#include "Pickups.hpp"

namespace INTERMISSION
{
	struct Data
	{
		static bool Active;
	};

	void Start();
	void Process();
}
