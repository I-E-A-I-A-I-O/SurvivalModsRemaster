#pragma once

#include "types.h"
#include <random>

namespace CALC
{
	int RanInt(int max, int min);
	size_t RanInt(size_t max, size_t min);
	bool IsInRange_2(Vector3 a, Vector3 b, float range);
}
