#include <View.h>
#include <QWheelEvent>

FlowView::FlowView(QGraphicsScene* scene)
	:QGraphicsView(scene, nullptr)
{
	setRenderHints(QPainter::Antialiasing |
		QPainter::TextAntialiasing |
		QPainter::SmoothPixmapTransform);

	setDragMode(QGraphicsView::ScrollHandDrag);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	setCacheMode(QGraphicsView::CacheBackground);
}

/**
 * @brief Sets zoom level based on scroll event
 *
 * @param event Scroll event
 */
void FlowView::wheelEvent(QWheelEvent* event)
{
	QPoint delta = event->angleDelta();

	if (delta.y() == 0)
	{
		event->ignore();
		return;
	}

	const float d = float(delta.y()) / std::abs(delta.y());

	if (d > 0.0f)
		scaleUp();
	else
		scaleDown();
}

/**
 * @brief Scales up view and checks for limit
 *
 */
void FlowView::scaleUp()
{
	constexpr double factor = 1.2;

	QTransform t = transform();
	if (t.m11() > 2.0)
		return;

	scale(factor, factor);
}

/**
 * @brief Scales down view and checks for limit
 *
 */
void FlowView::scaleDown()
{
	constexpr double factor = 1 / 1.2;
	QTransform t = transform();
	if (t.m11() < 0.2)
		return;
	scale(factor, factor);
}