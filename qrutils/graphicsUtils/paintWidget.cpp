#include <QMutableListIterator>

#include "paintWidget.h"
#include "painterInterface.h"

using namespace graphicsUtils;

PaintWidget::PaintWidget(QWidget *parent)
	: QWidget(parent)
	, mPainter()
{
}

void PaintWidget::paintEvent(QPaintEvent *event)
{
	QWidget::paintEvent(event);
	QPainter *painter = new QPainter(this);

	QMutableListIterator<PainterInterface*> i(mPainter);
	while(i.hasNext()){
		i.next()->paint(painter);
	}
}

void PaintWidget::setPainter(PainterInterface *p)
{
	mPainter.prepend(p);
}


