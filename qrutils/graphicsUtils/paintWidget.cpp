#include "paintWidget.h"
#include "painterInterface.h"

using namespace graphicsUtils;

PaintWidget::PaintWidget(QWidget *parent)
	: QWidget(parent)
{
}

void PaintWidget::paintEvent(QPaintEvent *event)
{
	QWidget::paintEvent(event);
	QPainter painter(this);

	for (PainterInterface * const painterIface : mPainters) {
		painterIface->paint(&painter);
	}
}

void PaintWidget::appendPainter(PainterInterface *painter)
{
	mPainters << painter;
}
