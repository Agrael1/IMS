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
	get(22, 45).PlantTree(TreeTy::oak, 1, 0.23f);
	get(22, 46).PlantTree(TreeTy::oak, 30, 10.23f);
	get(23, 45).PlantTree(TreeTy::oak, 10, 5.23f);
	get(23, 46).PlantTree(TreeTy::oak, 17, 7.56f);
	get(8, 6).PlantTree(TreeTy::pine, 1, 0.23f);
	get(8, 7).PlantTree(TreeTy::pine, 6, 2.29f);
	get(9, 5).PlantTree(TreeTy::pine, 10, 5.80f);
	get(9, 4).PlantTree(TreeTy::pine, 30, 10.23f);
	get(38, 25).PlantTree(TreeTy::pine, 4, 2.23f);
	get(38, 26).PlantTree(TreeTy::pine, 4, 2.23f);
	get(38, 27).PlantTree(TreeTy::pine, 4, 2.23f);
	get(39, 25).PlantTree(TreeTy::pine, 4, 2.23f);
	get(39, 26).PlantTree(TreeTy::pine, 4, 2.23f);

	Cell::SetCallbacks([&]() {trees++; }, 
		[&]() {trees--; printf("%zu\n", trees); });

	trees += 13;
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
	if (trees > 200)
		return false;
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

			size_t(x.GrowSeed());
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
