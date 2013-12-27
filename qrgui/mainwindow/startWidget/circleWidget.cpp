#include "circleWidget.h"

#include <QtWidgets/QVBoxLayout>
#include <QtGui/QPainter>

using namespace qReal;

CircleWidget::CircleWidget(QSize const &size, QWidget *innerWidget, QWidget *parent)
	: QWidget(parent)
{
	QRect const rect(QPoint(), size);
	QRegion const circleRegion(rect, QRegion::Ellipse);
	setMask(circleRegion);
	setFixedSize(size);

	QVBoxLayout * const circleLayout = new QVBoxLayout;
	circleLayout->setContentsMargins(0, 0, 0, 0);
	circleLayout->setSpacing(0);
	circleLayout->setMargin(0);
	circleLayout->addWidget(innerWidget, 0, Qt::AlignCenter);

	setLayout(circleLayout);
	// TODO: move it into brand manager styles
	setStyleSheet("background-color: white");
}

void CircleWidget::paintEvent(QPaintEvent *event)
{
	QWidget::paintEvent(event);
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	QPen borderPen(Qt::black);
	borderPen.setWidth(6);
	painter.setPen(borderPen);
	painter.drawEllipse(QRect(QPoint(), size()));
}
