#include "pch.h"
#include "Timers.hpp"

Timers::Timers()
{
	spawnStartTime = 0;
	spawnCurrentTime = 0;
	spawnTimerStarted = false;
}

bool Timers::SpawnTimer()
{
	if (!spawnTimerStarted)
	{
		spawnTimerStarted = true;
		spawnStartTime = GAMEPLAY::GET_GAME_TIMER();
		return false;
	}
	else
	{
		spawnCurrentTime = GAMEPLAY::GET_GAME_TIMER();
		if (spawnCurrentTime - spawnStartTime >= 300000)
		{
			return true;
		}
		return false;
	}
}
