#pragma once
#include <QGraphicsView>


class FlowView : public QGraphicsView
{
public:
	FlowView(QGraphicsScene* scene);
private:
	void wheelEvent(QWheelEvent* event) override;
	void scaleUp();
	void scaleDown();
};