#include "pch.h"
#include "Calculations.hpp"

int CALC::RanInt(int max, int min)
{
	std::random_device rd;
	std::default_random_engine eng(rd());
	std::uniform_int_distribution<int> distr(min, max);

	return distr(eng);
}

size_t CALC::RanInt(size_t max, size_t min)
{
	std::random_device rd;
	std::default_random_engine eng(rd());
	std::uniform_int_distribution<size_t> distr(min, max);

	return distr(eng);
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
