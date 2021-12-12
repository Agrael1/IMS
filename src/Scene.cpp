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
}

void FlowScene::Start()
{
	c->grid.Update();
}

bool FlowScene::StartFor(size_t time)
{
	return c->grid.UpdateFor(time);
}

void FlowScene::SendStats() const
{
	auto st = c->grid.GetStats();

	printf("Age of the system: %zu\n"
		"Current Trees: %zu\n"
		"Died from Starvation: %zu\n"
		"Died of old Age: %zu\n"
		"Population index: %.2f\n\n"
		,st.time, st.trees, st.trees_starved, st.trees_aged, 
		float(st.trees_starved + st.trees_aged)/float(st.trees_all));
}

void FlowScene::Refresh()
{
	c->grid.Zero();
}
