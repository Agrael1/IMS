#include <Scene.h>
#include <Grid.h>
#include <Node.h>
#include <QThread>


struct Core
{
	Core():ggrid(grid){}
	Grid grid;
	GGrid ggrid;
};

FlowScene::FlowScene()
	:c(std::make_unique<Core>())
{
	size_t vert = 0;
	for (size_t x = 0; auto & i : c->ggrid.field) {
		if (x == c->grid.width-1)
		{
			vert += 50;
			x = - 1;
		}
		i.setPos({float(x*50), float(vert)});
		addItem(&i);
		x++;
	}
}
FlowScene::~FlowScene()
{
	launch = false;
}

void FlowScene::Start()
{
	c->grid.Update();
}
