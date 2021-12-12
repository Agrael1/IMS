#pragma once
#include "Cell.h"
#include <vector>

constexpr std::string_view map =
"##############~~**********************************"
"##############~~**********************************"
"################~~#############*******************"
"######00000000###~~#########**********************"
"######00000000#####~~####*************************"
"######00000000#######~~###########################"
"########00000000##~~~~############################"
"########00000000######~~~~~~~~####################"
"############0000000#######~~~~~~~~~###############"
"############0000000###########~~~~~###############"
"##################00####~~~~~~~###################"
"####################00####~~~~~~##################"
"######################000~~~~~~###################"
"###################~~~~~00########################"
"#############~~~~~~#####00########################"
"########~~~~~~##############00####################"
"##~~~~~~~~##########********##00##################"
"************************##########################"
"**********************************################"
"###################*********************##########"
"#########****************#########################"
"##################################################"
;


//constexpr std::string_view map = "*#~0";
constexpr size_t width_map = 50;
constexpr size_t height_map = map.size() / width_map;

inline char sample(float width, float height)
{
	auto h = std::round(height * (height_map - 1));
	auto w = std::round(width * (width_map - 1));
	auto x = size_t(h * width_map + w);
	return map[x];
}


class Grid
{
	friend class GGrid;
public:
	struct Stats
	{
		size_t time = 0;
		size_t trees = 0;
		size_t trees_starved = 0;
		size_t trees_aged = 0;
		size_t trees_all = 0;
	};
public:
	static constexpr auto height = 20;
	static constexpr auto width = 20;
	static constexpr float hdelta = 1.0f / height;
	static constexpr float wdelta = 1.0f / width;
public:
	Grid(std::string_view map = map);
public:
	void Write()const;
	bool Update();
	bool UpdateFor(size_t maxtime);
	void SetCallback(const std::function<void(size_t, size_t)>& a)
	{
		update = a;
	}
	auto GetStats()const noexcept
	{
		return Stats{time, trees, trees_starved, trees_aged, trees_all};
	}
	void Zero()
	{
		time = 0;
		trees_starved = 0;
		trees_aged = 0;
		trees_all = trees;
	}
private:
	auto& get(size_t i, size_t j)
	{
		return grid[j * width + i];
	}
	auto& get_top(size_t i, size_t j)
	{
		if (j == 1)
			return get(i, 0);
		return reserves.front();
	}
	auto& get_left(size_t i, size_t j)
	{
		if (i == 1)
			return get(0, j);
		return reserves.at(reserves.size() - 2);
	}
	void trim()
	{
		if (reserves.size() > width - 2) //frame does not count
			reserves.pop_back();
	}
private:
	size_t time = 0;
	size_t trees = 0;
	size_t trees_starved = 0;
	size_t trees_aged = 0;
	size_t trees_all = 0;
	std::vector<Cell> reserves;
	std::array<Cell, width* height> grid;
	std::function<void(size_t, size_t)> update;
};