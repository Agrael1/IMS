#pragma once
#include <algorithm>

//water uptake 
// 1.43 mm/m2 oak
// 1.31 pine
struct Resources
{
	float Water;
public:
	void Produce(Resources prod, Resources max)
	{
		if (Water != max.Water)
			Water = std::clamp(Water + prod.Water, 0.0f, max.Water);
	}
	auto operator/(float a)const
	{
		return Resources{ Water / a };
	}
	auto operator+(const Resources& prod)const
	{
		return Resources{ Water + prod.Water };
	}
	auto operator*(float a)const
	{
		return Resources{ Water * a };
	}
	void Clamp(Resources max)
	{
		if (Water >= max.Water)
			Water = max.Water;
	}
};