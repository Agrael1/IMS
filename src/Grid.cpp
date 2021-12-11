#include "Grid.h"

Grid::Grid(std::string_view map)
{
	float xh = 0;
	float xw = 0;

	for (size_t i = 0, j = 0; j * height + i < height * width; )
	{
		auto* x = grid.data() + j * height + i;
		auto ty = Decode(sample(xw, xh));
		std::construct_at(x, ty, GetMaxrc(ty) / 8);

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

void Grid::Write() const
{
	for (size_t i = 0; auto && c: grid)
	{
		putchar(Encode(c.Type()));
		if (i == width - 1) { putchar('\n'); i = 0; continue; }
		i++;
	}
}

void Grid::Update()
{
	time++;
	//phases:
	//1) growth 

	//2) redistr by VonNeuman
	for (size_t i = 1; i < height - 1; i++)
	{
		for (size_t j = 1; j < width - 1; j++)
		{
			auto& x = get(j, i);
			reserves.push_back(x);
			x.UpdateResources(get_left(j, i), get(j + 1, i), get_top(j, i), get(j, i + 1));
			trim();
		}
	}
	//3) produce
	for (auto& i : grid)
	{
		i.Produce();
	}
}
