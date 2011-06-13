#include "robotItem.h"
#include <QtGui/QCursor>

#include <QDebug>

using namespace qReal::interpreters::robots;
using namespace details::d2Model;

int const border = 5;

RobotItem::RobotItem()
	: QGraphicsItem()
	, mImage(QImage(":/icons/robot.png"))
{
	setFlags(ItemIsSelectable | ItemIsMovable | ItemClipsChildrenToShape |
		ItemClipsToShape | ItemSendsGeometryChanges);

	setAcceptHoverEvents(true);
	setAcceptDrops(true);
	setCursor(QCursor(Qt::PointingHandCursor));
	setZValue(1);
}

void RobotItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *style, QWidget *widget)
{
	Q_UNUSED(style)
	Q_UNUSED(widget)

	painter->drawImage(QPoint(0,0), mImage);
}

QRectF RobotItem::boundingRect() const
{
	return QRect(0,0,robotWidth, robotHeight).adjusted(-border, -border, border, border);
}
