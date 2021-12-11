#pragma once
#include "Cell.h"

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
	static constexpr auto height = 50;
	static constexpr auto width = 50;
	static constexpr float hdelta = 1.0f / height;
	static constexpr float wdelta = 1.0f / width;
public:
	Grid(std::string_view map)
	{
		float xh = 0;
		float xw = 0;

		for (size_t i = 0, j = 0; j * height + i < height * width; )
		{
			auto* x = grid.data() + j * height + i;
			std::construct_at(x, Decode(sample(xw, xh)), Resources{}, Resources{});

			if (i == width - 1)
			{
				i = 0;
				xw = 0;
				j++;
				xh += hdelta;
				continue;
			}
			xw += wdelta;
			i++;
		}
	}
public:
	void Write()const
	{
		for (size_t i = 0; auto && c: grid)
		{
			putchar(Encode(c.Type()));
			if (i == width - 1) { putchar('\n'); i = 0; continue; }
			i++;
		}
	}
private:
	std::array<Cell, width* height> grid;
};