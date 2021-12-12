#pragma once
#include "Resources.h"
#include "Tree.h"
#include <optional>
#include <array>
#include <functional>

#define ENUM_SOIL() X(chernoz) X(seroz) X(water) X(rock)

enum class SoilTy : uint8_t
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
	return SoilMax<SoilTy::rock>::rc;
}


// https://www.researchgate.net/profile/M-D-Swaine/publication/231995620_Tree_population_dynamics_at_Kade_Ghana_1968-1982/links/00b49528f5b9f99875000000/Tree-population-dynamics-at-Kade-Ghana-1968-1982.pdf
constexpr Resources sample_production{
	.Water = 1.64f * SoilMax<SoilTy::water>::ro * cell_w * cell_w
};

class Random
{
public:
	static auto& get()
	{
		static Random r;
		return r.rng;
	}
private:
	std::mt19937 rng;
};


class Cell
{
public:
	enum class Reason
	{
		Starvation,
		Age,
		Premature
	};
public:
	using von_neuman = struct
	{
		Cell& left; Cell& right; Cell& up; Cell& down;
	};
	struct XMoore
	{
		Cell* tl; Cell* tt; Cell* tr;
		Cell* l;		    Cell* r;
		Cell* bl; Cell* bb;	Cell* br;
	};
	using moore = union {
		XMoore m; std::array<Cell*, 8> list;
	};
public:
	Cell() = default;
	Cell(SoilTy st, Resources xrc, Resources production = sample_production)noexcept
		:st(st), rc(xrc), rc_max(GetMaxrc(st)), rc_production(production)
	{
		if (st == SoilTy::water)
		{
			rc = SoilMax<SoilTy::water>::rc;
			rc_production = SoilMax<SoilTy::water>::rc;
		}
	}
public:
	auto& GetTree()const noexcept
	{
		return tree;
	}
	auto Type()const noexcept
	{
		return st;
	}
	bool Updates()const noexcept
	{
		return updates;
	}
	void DoneUpd()noexcept
	{
		updates = false;
	}
	template<typename ...Args>
	void PlantTree(bool upd, Args&& ...args)
	{
		if (st == SoilTy::water|| st == SoilTy::rock)return;
		tree.emplace(std::forward<Args>(args)...);
		if (OnTreePlant)OnTreePlant();
	}
	template<typename ...Args>
	void PlantTree(Args&& ...args)
	{
		if (st == SoilTy::water)return;
		tree.emplace(std::forward<Args>(args)...);
		if (OnTreePlant)OnTreePlant();
		updates = true;
	}

	void OnDeath(Reason r)
	{
		if (OnTreeDies)OnTreeDies(r);
		updates = true;
		died = true;
	}
	void TreeTake()
	{
		if (!tree) return;
		if (tree->Dies())
		{
			tree.reset();
			OnDeath(Reason::Age);
			return;
		}
		rc -= tree->GetUptake();
		(*tree)++;
		if (!tree->HasRoots() && rc.Water < 0) {
			Reason r = tree->Age() < 4 ? Reason::Premature : Reason::Starvation;
			tree.reset();
			OnDeath(Reason::Starvation);
			rc.Water = 0.0f;
		}
	}
	void Compensate(von_neuman eps)
	{
		if (!tree) return;
		if (rc.Water > 0)
		{
			tree->Grow(Random::get());
			return;
		}


		float sum = rc.Water;
		auto xnul = [&](float* a) {if (*a > 0.0f) { rc.Water += *a; *a = 0.0f; } };
		auto xsum = [&](float* a) {sum += *a > 0 ? *a : 0; };


		std::array<float*, 4> x = { &eps.down.rc.Water, &eps.up.rc.Water, &eps.left.rc.Water, &eps.right.rc.Water };
		std::for_each(x.begin(), x.end(), xsum);

		if (sum < 0)
		{
			for (auto i : x)
				xnul(i);
			if (!tree->Survives(rc))
			{
				rc.Water = 0.0f;
				Reason r = tree->Age() < 4 ? Reason::Premature : Reason::Starvation;
				tree.reset();
				return OnDeath(r);
			}
			rc.Water = 0.0f;
			return;
		}

		float minw = *x[0];
		for (auto i : x)
			minw = std::min(*i, minw);
		if (minw >= rc.Water / 4)
		{
			eps.down.rc.Water += rc.Water / 4;
			eps.up.rc.Water += rc.Water / 4;
			eps.left.rc.Water += rc.Water / 4;
			eps.right.rc.Water += rc.Water / 4;
			tree->Grow(Random::get());
			return;
		}


		float rest = 4.0f;
		float f = rc.Water / rest;
		std::sort(x.begin(), x.end(), [](auto l, auto r) {return *l < *r; });
		for (auto i : x)
		{
			if (*i <= 0)
			{
				rest -= 1.0f;
				continue;
			}
			f = rc.Water / rest;
			if (*i > -f)
			{
				*i += f;
				rest -= 1.0f;
				rc.Water -= f;
				continue;
			}
			rc.Water += *i;
			*i = 0.0f;
			rest -= 1.0f;
		}
		tree->Grow(Random::get());
	}
	void TreeReproduce(moore eps)
	{
		if (!tree)return;
		auto repr_c = tree->Reproduction();
		if (repr_c == 0.0f)return;

		for (auto i : eps.list)
		{
			float r = std::uniform_real_distribution<float>{ 0.0f, 1.0f }(Random::get());
			if (r > repr_c)continue;
			i->AddSeed(tree->GetType());
		}
	}

	void AddSeed(TreeTy ty)
	{
		seeds[size_t(ty)]++;
	}
	bool GrowSeed()
	{
		if (died){
			died = false;
			return false;
		}
		if (!seeds[0] && !seeds[1])return false;
		if (tree || st == SoilTy::water || st == SoilTy::rock)
		{
			memset(seeds.data(), 0, sizeof(seeds));
			return false;
		}
		if (!seeds[0])
		{
			PlantTree(TreeTy::pine);
			return true;
		}
		if (!seeds[1])
		{
			PlantTree(TreeTy::oak);
			return true;
		}
		float r = std::uniform_real_distribution<float>{ 0.0f, 1.0f }(Random::get());
		if (r > 0.5){
			PlantTree(TreeTy::oak);
		}
		else{
			PlantTree(TreeTy::pine);
		}
		return true;

	}
	void UpdateResources(const von_neuman eps)
	{
		rc = rc / 2 + eps.left.rc / 8 + eps.right.rc / 8 + eps.up.rc / 8 + eps.down.rc / 8;
		rc.Clamp(rc_max);
	}
	void Produce()
	{
		rc.Produce(rc_production, rc_max);
	}
public:
	static void SetCallbacks(std::function<void()> xOnTreePlant, std::function<void(Reason)> xOnTreeDies)
	{
		OnTreePlant = xOnTreePlant;
		OnTreeDies = xOnTreeDies;
	}
private:
	SoilTy st;
	bool updates = false;
	bool died = false;
	Resources rc;
	Resources rc_max;
	Resources rc_production;
	std::optional<Tree> tree;
	std::array<size_t, size_t(TreeTy::Count)> seeds{ 0 };

	static std::function<void()> OnTreePlant;
	static std::function<void(Reason)> OnTreeDies;
};

#undef ENUM_SOIL