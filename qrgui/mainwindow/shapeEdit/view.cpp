#include "view.h"

View::View(QWidget * parent) : QGraphicsView(parent)
{
}

void View::zoomIn()
{
	scale(1.5,1.5);
}

void View::zoomOut()
{
	scale(0.66,0.66);
}

void View::wheelEvent(QWheelEvent *event)
{
	QGraphicsView::wheelEvent(event);
	if (event->buttons() == Qt::MidButton) {
		if (event->delta() >= 0)
			 zoomOut();
		else
			zoomIn();
	}
}

void View::keyPressEvent(QKeyEvent *event)
{
	QGraphicsView::keyPressEvent(event);
	if (event->matches(QKeySequence::Delete))
		emit deleteItem();
}
