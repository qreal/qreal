#include "circleWidget.h"

#include <QtWidgets/QVBoxLayout>
#include <QtGui/QPainter>

using namespace qReal;

CircleWidget::CircleWidget(QSize const &size, QWidget *innerWidget, QWidget *parent)
	: QWidget(parent)
{
	QVBoxLayout * const circleLayout = new QVBoxLayout;
	circleLayout->setContentsMargins(0, 0, 0, 0);
	circleLayout->setSpacing(0);
	circleLayout->setMargin(0);
	circleLayout->addWidget(innerWidget, 0, Qt::AlignCenter);

	setFixedSize(size);
	setLayout(circleLayout);
}

void CircleWidget::paintEvent(QPaintEvent *event)
{
	QWidget::paintEvent(event);
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	QPen borderPen(palette().foreground().color());
	borderPen.setWidth(3);
	QBrush const backgroundBrush(Qt::white);
	painter.setBrush(backgroundBrush);
	painter.setPen(borderPen);
	painter.drawEllipse(QRect(QPoint(2, 2), size() - QSize(4, 4)));
}
