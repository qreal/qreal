#include <QtGui/QPaintEngine>
#include <QtGui/QPaintEvent>

#include "rootWidget.h"

using namespace Ui::WidgetsEdit;

RootWidget::RootWidget()
	: QWidget(), mRenderer(NULL)
{
}

void RootWidget::setShape(const QDomDocument &shape)
{
	mShape = shape;
	mRenderer = new SdfRenderer(shape);
}

QDomDocument RootWidget::shapeDocument() const
{
	return mShape;
}

void RootWidget::paintEvent(QPaintEvent *event)
{
	if (mRenderer) {
		QPainter *painter = new QPainter;
		painter->begin(this);
		mRenderer->render(painter, geometry().translated(-geometry().topLeft()));
		painter->end();
	}
	event->accept();
}
