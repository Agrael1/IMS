#include "Node.h"
#include <Grid.h>
#include <QPainter>


class Pics
{
public:
	Pics()
		:_oak(QImage{ ":/oak.png" }.scaled(50, 50)),
		_pine(QImage{ ":/pine.png" }.scaled(50, 50))
	{

	}
	static auto& get()
	{
		static Pics p;
		return p;
	}
	static QImage& oak()
	{
		return get()._oak;
	}
	static QImage& pine()
	{
		return get()._pine;
	}
private:
	QImage _oak;
	QImage _pine;
};


Node::Node(Cell& c)
	:c(c)
{
	switch (c.Type())
	{
	case SoilTy::chernoz:
		b = QColor{ "black" };
		break;
	case SoilTy::seroz:
		b = QColor{ "brown" };
		break;
	case SoilTy::water:
		b = QColor{ "#ADD8E6" };
		break;
	case SoilTy::rock:
		b = QColor{ "grey" };
		break;
	default:
		break;
	}
	resize({ sz,sz });
	tree = QImage{};
	GetTree();
}

void Node::GetTree()
{
	auto& x = c.GetTree();
	if (!x)tree = QImage{};
	else MakeTree();
}

void Node::DrawBackground(QPainter* painter)
{
	painter->setBrush(b);
	painter->drawRect(QRect{ 1,1,sz - 1, sz - 1 });

	if (!tree.isNull())
		painter->drawImage(QRect{ 1,1,sz - 1, sz - 1 }, tree);
}

void Node::MakeTree()
{
	auto& tr = c.GetTree();
	switch (tr->GetType())
	{
	case TreeTy::oak:
		tree = QImage{ ":/oak.png" }.scaled(50, 50);
		break;
	case TreeTy::pine:
		tree = QImage{ ":/pine.png" }.scaled(50, 50);
		break;
	default:
		break;
	}
}


/////////////////////////////////////////////////////////////////////
GGrid::GGrid(Grid& c)
{
	field.reserve(c.height * c.width);
	for (auto& i : c.grid) {
		field.emplace_back(i);
	}

	c.SetCallback([this](size_t i, size_t j) {Update(i, j); });
}

void GGrid::Update(size_t i, size_t j)
{
	field[j * Grid::width + i].Update();
}
