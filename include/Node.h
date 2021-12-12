#pragma once
#include <QGraphicsWidget>
#include <vector>

class Cell;
class Grid;



class Node : public QGraphicsWidget
{
public:
	static constexpr auto sz = 50;
public:
	Node(Cell& c);
	Node(Node&& other)noexcept
		:c(other.c), b(other.b), tree(std::move(other.tree))
	{

	}
public:
	virtual void paint(QPainter* painter,
		const QStyleOptionGraphicsItem* option,
		QWidget* widget = nullptr) override
	{
		DrawBackground(painter);

		QGraphicsWidget::paint(painter, option, widget);
	}
	void GetTree();
	void Update()
	{
		GetTree();
		update();
	}
private:
	void DrawBackground(QPainter* painter);
	void MakeTree();
private:
	Cell& c;
	QBrush b;
	QImage tree;
};

struct GGrid
{
	GGrid(Grid& c);

	void Update(size_t i, size_t j);
public:
	std::vector<Node> field;
};
