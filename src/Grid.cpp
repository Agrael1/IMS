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
	//TreeTy type, size_t age, float height
	get(10, 11).PlantTree(false, TreeTy::oak, 1, 0.23f);
	get(10, 12).PlantTree(false, TreeTy::pine, 30, 10.23f);
	get(11, 11).PlantTree(false, TreeTy::oak, 10, 5.23f);
	get(11, 12).PlantTree(false, TreeTy::oak, 301, 7.56f);
	get(8, 6).PlantTree(false, TreeTy::pine, 301, 0.23f);
	get(8, 7).PlantTree(false, TreeTy::pine, 6, 2.29f);
	get(9, 5).PlantTree(false, TreeTy::oak, 10, 5.80f);
	get(9, 4).PlantTree(false, TreeTy::pine, 30, 10.23f);
	get(14, 1).PlantTree(false, TreeTy::pine, 4, 2.23f);
	get(14, 2).PlantTree(false, TreeTy::pine, 4, 2.23f);
	get(14, 3).PlantTree(false, TreeTy::oak, 4, 2.23f);
	get(15, 1).PlantTree(false, TreeTy::pine, 4, 2.23f);
	get(15, 5).PlantTree(false, TreeTy::oak, 4, 2.23f);

	Cell::SetCallbacks([&]() {trees++; trees_all++; },
		[&](Cell::Reason r) {
			trees--; 
			switch (r)
			{
			case Cell::Reason::Starvation:
				trees_starved++;
				break;
			case Cell::Reason::Age:
				trees_aged++;
				break;
			default:
				break;
			}
		});
	trees = 13;
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

bool Grid::Update()
{
	time++;
	//phases:
	//1) growth 

	for (size_t j = 1; j < height - 1; j++)
	{
		for (size_t i = 1; i < width - 1; i++)
		{
			auto& x = get(i, j);
			x.TreeTake();
			// compensate if not enough
			x.Compensate({ get(i - 1, j), get(i + 1, j), get(i, j - 1), get(i, j + 1) });

			x.TreeReproduce({
				&get(i - 1, j - 1), &get(i, j - 1), &get(i + 1, j - 1),
				&get(i - 1, j),				&get(i + 1, j),
				&get(i - 1, j + 1), &get(i, j + 1), &get(i + 1, j + 1) });

			x.GrowSeed();
			if (x.Updates())
			{
				update(i, j);
				x.DoneUpd();
			}

		}
	}

	//2) redistr by VonNeuman
	for (size_t i = 1; i < height - 1; i++)
	{
		for (size_t j = 1; j < width - 1; j++)
		{
			auto& x = get(j, i);
			reserves.push_back(x);
			x.UpdateResources({ get_left(j, i), get(j + 1, i), get_top(j, i), get(j, i + 1) });
			trim();
		}
	}
	//3) produce
	for (auto& i : grid)
	{
		i.Produce();
	}
	return true;
}

bool Grid::UpdateFor(size_t maxtime)
{
	if (time != maxtime)
		return Update();
	return false;
}
