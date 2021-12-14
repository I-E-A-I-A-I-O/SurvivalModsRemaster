#pragma once

#include "SpawnData.hpp"
#include "Timers.hpp"
#include <vector>

namespace STORAGE
{
	struct StoredData
	{
		static std::vector<SpawnData> peds;
		static std::vector<Timers> timers;
	};
}
