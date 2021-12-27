#pragma once

#include "Controls.hpp"
#include "natives.h"
#include "Screen.hpp"
#include "Calculations.hpp"
#include "Survival.hpp"
#include "Initialization.hpp"
#include <map>
#include <vector>
#include <string>

struct Data
{
	static Hash enemiesRelGroup;
	static int intermissionDuration;
	static Controls tenWaveControl;
	static Controls infiniteWaveControl;
	static Controls timedSurvivalControl;
	static Controls hardcoreSurvivalControl;
	static Controls cancelControl;
	static Controls reloadTriggerPedsControl;
	static bool canStartMission;
	static int TPIndex;
};

struct EntityPosition
{
	Vector3 coords;
	float heading;
};

struct TriggerPedsData
{
	static std::vector<std::string> names;
	static std::vector<std::string> models;
	static std::vector<Ped> peds;
	static std::vector<EntityPosition> positions;
	static std::vector<Blip> blips;
	static std::vector<bool> timerActive;
	static std::vector<int> starTime;
	static std::vector<std::string> tasks;

	static void ClearTriggerPeds()
	{
		size_t size = blips.size();

		for (size_t i = 0; i < size; i++)
		{
			Blip blip = blips.at(i);
			Ped ped = peds.at(i);

			if (blip != 0)
				UI::REMOVE_BLIP(&blip);

			if (ped != 0)
				ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&ped);
		}

		names.clear();
		models.clear();
		positions.clear();
		blips.clear();
		timerActive.clear();
		starTime.clear();
		starTime.clear();
		tasks.clear();
		peds.clear();
	}
};

void ScriptMain();
void OnAbort();
