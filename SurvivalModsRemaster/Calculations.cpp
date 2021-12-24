#include "pch.h"
#include "Calculations.hpp"
#include <cstdlib>
#include <ctime>

int CALC::RanInt(int max, int min)
{
	return min + (std::rand() % (max - min + 1));
}

size_t CALC::RanInt(size_t max, size_t min)
{
	return min + (std::rand() % (max - min + 1));
}

bool CALC::IsInRange_2(Vector3 a, Vector3 b, float range)
{
	Vector3 dif = Vector3();
	dif.x = a.x - b.x;
	dif.y = a.y - b.y;
	dif.z = a.z - b.z;

	float length = (dif.x * dif.x) + (dif.y * dif.y) + (dif.z * dif.z);

	return sqrt(length) < range;
}
