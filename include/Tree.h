#pragma once
#include "Resources.h"
#include <random>
#include <cassert>
#include <numeric>

#define ENUM_TREE() X(oak) X(pine)

enum class TreeTy
{
#define X(a) a,
	ENUM_TREE()
#undef X
	Count
};

constexpr auto year = 365;



//https://www.researchgate.net/publication/311535694_Comparison_of_Water_Consumption_by_Needle_and_Broad-_Leaved_Tree_Species_in_the_First_Month_of_Growing_Season
template <TreeTy T>struct TreeMax {
	static constexpr bool defined = false;
};
template<>struct TreeMax<TreeTy::oak> {
	static constexpr bool defined = true;
	static constexpr float height = 30.5f;
	static constexpr size_t age = 450;
	static constexpr size_t root_ac = 27; //https://www.fs.fed.us/pnw/olympia/silv/oak-studies/oak-roots.shtml
	static constexpr size_t repr_age = 35;
	static constexpr float seed_chance = .70f;
	static constexpr Resources uptake_norm{
		.Water = 0.05f * year
	};
	static constexpr Resources uptake_coeff{
		.Water = (52.55f * year - uptake_norm.Water) / height
	};
	static constexpr Resources uptake_survival{
		.Water = 1.4f / 1.91f
	};

	static float growth_rate(size_t age, std::mt19937& rng)
	{
		if (age > 120)return 0.0f;
		if (age < 10)
			return std::uniform_real_distribution<float>{0.30f, 0.35f}(rng);
		return std::uniform_real_distribution<float>{1.0f, 1.5f}(rng);
	}
};
template<>struct TreeMax<TreeTy::pine> {
	static constexpr bool defined = true;
	static constexpr float height = (63.0f + 45.0f) / 2.0f;
	static constexpr float seed_chance = .55f;
	static constexpr size_t root_ac = 25; //https://zen.yandex.ru/media/id/5c07e1a9ded11e00aa96001f/korni-sosny-dlinoi-48-kilometrov-5ce579d3ef774e00b33271ef
	static constexpr size_t age = 300;
	static constexpr size_t repr_age = 20; //https://www.fs.fed.us/database/feis/plants/tree/pinalb/all.html#:~:text=Cones%20are%201st%20produced%20at,trees%20still%20reproduce%20%5B207%5D.
	static constexpr Resources uptake_norm{
		.Water = 0.08f * year
	};
	static constexpr Resources uptake_coeff{
		.Water = (57.83f * year - uptake_norm.Water) / height
	};
	static constexpr Resources uptake_survival{
		.Water = 3.7f / 3.93f
	};

	// 0-5 years = 10cm , 5-10 years = until 60 cm , 10 - .. = 0,6-1m:pine http://ogorodsadovod.com/entry/2585-skolko-rastet-sosna-pri-vyrashchivanii-na-uchastke#:~:text=%D0%91%D0%BE%D0%BB%D1%8C%D1%88%D0%B8%D0%BD%D1%81%D1%82%D0%B2%D0%BE%20%D1%81%D0%BE%D1%81%D0%B5%D0%BD%2C%20%D1%82%D0%B0%D0%BA%D0%B8%D1%85%20%D0%BA%D0%B0%D0%BA%20%D1%81%D0%BE%D1%81%D0%BD%D0%B0,6%20%D0%B4%D0%BE%201%20%D0%BC%20%D0%B5%D0%B6%D0%B5%D0%B3%D0%BE%D0%B4%D0%BD%D0%BE.
	static float growth_rate(size_t age, std::mt19937& rng)
	{
		if (age > 50)return 0.0f;
		if (age < 5)
			return std::uniform_real_distribution<float>{0.02f, 0.1f}(rng);
		if (age < 10)
			return std::uniform_real_distribution<float>{0.1f, 0.6f}(rng);
		return std::uniform_real_distribution<float>{0.6f, 1.0f}(rng);
	}
};

#define X(a) static_assert(TreeMax<TreeTy::a>::defined, "forgot to define " #a);
ENUM_TREE()
#undef X


template<template<TreeTy> class F, typename... Args>
static constexpr auto Bridge(TreeTy type, Args&&... args) noexcept
{
	switch (type)
	{
#define X(el) case TreeTy::el: return F<TreeTy::el>::Exec( std::forward<Args>( args )... );
		ENUM_TREE()
#undef X
	}
	assert("Invalid element type" && false);
}

template<TreeTy T>
struct GrowthRate
{
	static constexpr auto Exec(size_t age, std::mt19937& rng) noexcept
	{
		return TreeMax<T>::growth_rate(age, rng);
	}
};
template<TreeTy T>
struct MinUptake
{
	static constexpr auto Exec() noexcept
	{
		return TreeMax<T>::uptake_norm;
	}
};
template<TreeTy T>
struct MaxHeight
{
	static constexpr auto Exec() noexcept
	{
		return TreeMax<T>::height;
	}
};
template<TreeTy T>
struct Uptake
{
	static constexpr auto Exec(float height) noexcept
	{
		return TreeMax<T>::uptake_coeff * height;
	}
};
template<TreeTy T>
struct UptakeSurv
{
	static constexpr auto Exec() noexcept
	{
		return TreeMax<T>::uptake_survival;
	}
};
template<TreeTy T>
struct RootAdj
{
	static constexpr auto Exec(size_t age) noexcept
	{
		return TreeMax<T>::root_ac < age;
	}
};
template<TreeTy T>
struct Repr
{
	static constexpr auto Exec(size_t age) noexcept
	{
		return TreeMax<T>::repr_age < age?TreeMax<T>::seed_chance:0;
	}
};
template<TreeTy T>
struct XDies
{
	static constexpr auto Exec(size_t age) noexcept
	{
		return TreeMax<T>::age < age;
	}
};

class Cell;

//this is my tree, my tree is amazing, give it a lick
class Tree
{
public:
	using von_neuman = struct
	{
		Cell& left; Cell& right; Cell& up; Cell& down;
	};
public:
	//sappling ctor
	constexpr Tree(TreeTy type)
		:type(type), uptake(Bridge<MinUptake>(type))
	{}
	constexpr Tree(TreeTy type, size_t age, float height)
		: type(type), age(age), height(std::min(Bridge< MaxHeight >(type), height))
		, uptake(Bridge<Uptake>(type, height))
	{}
public:
	auto GetType() const noexcept
	{
		return type;
	}
	float Reproduction()const noexcept
	{
		return Bridge<Repr>(type, age);
	}
	auto Age()const noexcept
	{
		return age;
	}
	bool Dies()const noexcept
	{
		return Bridge<XDies>(type, age);
	}
	bool HasRoots()const noexcept
	{
		return Bridge<RootAdj>(type, age);
	}
	void Grow(std::mt19937& rng)
	{
		height += Bridge< GrowthRate >(type, age++, rng);
		uptake = Bridge<Uptake>(type, height);
	}
	bool Survives(Resources res)
	{
		return (res + (uptake - uptake * Bridge<UptakeSurv>(type))).Water > 0;
	}
	Resources GetUptake()const noexcept
	{
		return uptake;
	}
	void operator++(int)
	{
		age++;
	}
private:
	TreeTy type;
	float height = 0.0f;
	Resources uptake;
	size_t age = 0;
};

#undef ENUM_TREE