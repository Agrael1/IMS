#pragma once
#include "Resources.h"

#define ENUM_TREE() X(oak) X(pine)

enum class TreeTy
{
#define X(a) a,
	ENUM_TREE()
#undef X
};


//https://www.researchgate.net/publication/311535694_Comparison_of_Water_Consumption_by_Needle_and_Broad-_Leaved_Tree_Species_in_the_First_Month_of_Growing_Season
template <TreeTy T>struct TreeMax {
	static constexpr bool defined = false;
};
template<>struct TreeMax<TreeTy::oak> {
	static constexpr bool defined = true;
	static constexpr float height = 30.5f;
	static constexpr Resources uptake_norm{
		.Water = 0.05f
	};
	static constexpr Resources uptake_coeff{
		.Water = (52.55f - uptake_norm.Water) / height
	};
	static constexpr Resources uptake_survival{
		.Water = 1.4f / 1.91f
	};
};
template<>struct TreeMax<TreeTy::pine> {
	static constexpr bool defined = true;
	static constexpr float height = (63.0f + 45.0f) / 2.0f;
	static constexpr Resources uptake_norm{
		.Water = 0.08f
	};
	static constexpr Resources uptake_coeff{
		.Water = (57.83f - uptake_norm.Water) / height
	};
	static constexpr Resources uptake_survival{
		.Water = 3.7f / 3.93f
	};
};

#define X(a) static_assert(TreeMax<TreeTy::a>::defined, "forgot to define " #a);
ENUM_TREE()
#undef X


constexpr auto GetMinUptake(TreeTy t)
{
	switch (t)
	{
#define X(a) case TreeTy::a: return TreeMax<TreeTy::a>::uptake_norm;
		ENUM_TREE()
#undef X
	}
}

class Tree
{
public:
	constexpr Tree(TreeTy type)
		:type(type), uptake(GetMinUptake(type)) 
	{}
private:
	TreeTy type;
	float height = 0.0f;
	Resources uptake;
};