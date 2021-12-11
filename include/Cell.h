#pragma once
#include "Resources.h"
#include "Tree.h"
#include <optional>
#include <array>

#define ENUM_SOIL() X(chernoz) X(seroz) X(water) X(rock)

enum class SoilTy
{
#define X(a) a,
	ENUM_SOIL()
#undef X
};

constexpr SoilTy Decode(char c)
{
	switch (c)
	{
	case '#':return SoilTy::chernoz;
	case '*':return SoilTy::seroz;
	case '~':return SoilTy::water;
	case '0':return SoilTy::rock;
	}
	return SoilTy::rock;
}
constexpr char Encode(SoilTy c)
{
	switch (c)
	{
	case SoilTy::chernoz: return  '#';
	case SoilTy::seroz:return  '*';
	case SoilTy::water:return  '~';
	case SoilTy::rock:return  '0';
	}
	return '0';
}


constexpr float cell_w = 3.0f;
constexpr float cell_V = cell_w * cell_w * cell_w;

constexpr float Mass(float ro)noexcept
{
	return ro * cell_V;
}


// water = https://ru.wikipedia.org/wiki/%D0%92%D0%BE%D0%B4%D0%BD%D1%8B%D0%B9_%D1%80%D0%B5%D0%B6%D0%B8%D0%BC_%D0%BF%D0%BE%D1%87%D0%B2
// ro = https://propozitsiya.com/plotnost-pochvy-pri-razlichnyh-sistemah-ee-obrabotki
template <SoilTy T>struct SoilMax {
	static constexpr bool defined = false;
};
template<>struct SoilMax<SoilTy::chernoz> {
	static constexpr bool defined = true;
	static constexpr float ro = 1200.0f;
	static constexpr Resources rc{
		.Water = (61.9f + 38.3f + 32.5f) / 300.0f * Mass(ro)
	};
};
template<>struct SoilMax<SoilTy::seroz> {
	static constexpr bool defined = true;
	static constexpr float ro = 1270.0f;
	static constexpr Resources rc{
		.Water = (34.1f + 28.4f + 26.8f) / 300.0f * Mass(ro)
	};
};
template<>struct SoilMax<SoilTy::water> {
	static constexpr bool defined = true;
	static constexpr float ro = 1000.0f;

	static constexpr Resources rc{
		.Water = Mass(ro)
	};
};
template<>struct SoilMax<SoilTy::rock> {
	static constexpr bool defined = true;
	static constexpr Resources rc{
		.Water = 0
	};
};

#define X(a) static_assert(SoilMax<SoilTy::a>::defined, "forgot to define " #a);
ENUM_SOIL()
#undef X

constexpr auto GetMaxrc(SoilTy t)
{
	switch (t)
	{
#define X(a) case SoilTy::a: return SoilMax<SoilTy::a>::rc;
		ENUM_SOIL()
#undef X
	}
}


// https://www.researchgate.net/profile/M-D-Swaine/publication/231995620_Tree_population_dynamics_at_Kade_Ghana_1968-1982/links/00b49528f5b9f99875000000/Tree-population-dynamics-at-Kade-Ghana-1968-1982.pdf
constexpr Resources sample_production {
	.Water = 1.64f * SoilMax<SoilTy::water>::ro * cell_w * cell_w
};


class Cell
{
public:
	constexpr Cell() = default;
	constexpr Cell(SoilTy st, Resources rc, Resources production)noexcept
		:st(st), rc(rc), rc_max(GetMaxrc(st)), rc_production(production)
	{}
public:
	auto Type()const noexcept
	{
		return st;
	}
private:
	SoilTy st;
	Resources rc;
	Resources rc_max;
	Resources rc_production;
	std::optional<Tree> tree;
	std::array<size_t, size_t(TreeTy::Count)> seeds{0};
};

#undef ENUM_SOIL