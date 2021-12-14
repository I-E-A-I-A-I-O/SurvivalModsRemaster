#pragma once

#include "SpawnData.hpp"
#include "natives.h"
#include "json.hpp"
#include "Blip.hpp"
#include <fstream>
#include <string>
#include <map>
#include <vector>

namespace INIT
{
	struct InitData
	{
		static std::map<std::string, SpawnData> missionTriggerPeds;
	};

	std::map<std::string, Ped> SpawnMissionTriggerPeds();
	void LoadModel(Hash model);
	Hash LoadModel(const char* modelName);
	void UnloadModel(Hash model);
	Ped SpawnTriggerPed(std::string id);
	void LoadTriggerPeds();
}
