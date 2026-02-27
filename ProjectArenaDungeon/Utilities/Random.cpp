#include "pch.h"
#include "Utilities/Random.h"

#include <random>
#include <utility>	// std::swap

namespace
{
std::mt19937 gen;
}

namespace Random
{
void Init()
{
	std::random_device rd;
	gen.seed(rd());
}

float GetFloatZeroToOne()
{
	std::uniform_real_distribution<float> dis(0.0f, 1.0f);
	return dis(gen);
}

float Range(float min, float max)
{
	if (min > max) std::swap(min, max);

	std::uniform_real_distribution<float> dis(min, max);
	return dis(gen);
}

int Range(int min, int max)
{
	if (min > max) std::swap(min, max);

	std::uniform_int_distribution<int> dis(min, max);
	return dis(gen);
}

Color GetColor()
{
	return Color(GetFloatZeroToOne(), GetFloatZeroToOne(), GetFloatZeroToOne(), 1.0f);
}

Vector2 GetVector2(float min, float max)
{
	return Vector2(Range(min, max), Range(min, max));
}
}