#pragma once

#include "SpawnData.hpp"
#include "Script.hpp"
#include "natives.h"
#include "nlohmann/json.hpp"
#include "Blip.hpp"
#include <fstream>
#include <string>
#include <map>
#include <vector>

namespace INIT
{
	void LoadModel(Hash model);
	Hash LoadModel(const char* modelName);
	void UnloadModel(Hash model);
	void LoadTriggerPeds();
	Ped SpawnTriggerPed(size_t index);
}
