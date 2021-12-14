#pragma once

#include "Controls.hpp"
#include "natives.h"
#include "Screen.hpp"
#include "Calculations.hpp"
#include "Survival.hpp"
#include "Initialization.hpp"
#include <map>
#include <string>

struct Data
{
	static std::map<std::string, Ped> missionTriggerPeds;
	static std::string currentPedKey;
	static Hash enemiesRelGroup;
	static int intermissionDuration;
};

void ScriptMain();
void OnAbort();
