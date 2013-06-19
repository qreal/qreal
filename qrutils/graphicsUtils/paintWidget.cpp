#include "paintWidget.h"
#include "painterInterface.h"

using namespace graphicsUtils;

PaintWidget::PaintWidget(QWidget *parent)
	: QWidget(parent)
	, mPainters()
{
}

void PaintWidget::paintEvent(QPaintEvent *event)
{
	QWidget::paintEvent(event);
	QPainter painter(this);

	foreach (PainterInterface * const painterIface, mPainters) {
		painterIface->paint(&painter);
	}
}

void PaintWidget::setPainter(PainterInterface *painter)
{
	mPainters.prepend(painter);
}
