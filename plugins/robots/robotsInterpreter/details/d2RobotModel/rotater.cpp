#include "rotater.h"

#include <QtGui/QCursor>
#include <QDebug>

using namespace qReal::interpreters::robots;
using namespace details::d2Model;

Rotater::Rotater() : QGraphicsItem()
{
	setFlags(ItemIsSelectable | ItemIsMovable | ItemClipsChildrenToShape |
		ItemClipsToShape | ItemSendsGeometryChanges);

//	mLine.setParentItem(this);

	setAcceptHoverEvents(true);
	setAcceptDrops(true);
	setCursor(QCursor(Qt::PointingHandCursor));
	setZValue(2);

	mFrom = QPointF(0,0);
	mTo = QPointF(0,0);
	mLine.setLine(QLine(mFrom.toPoint(), mTo.toPoint()));
}

void Rotater::setMasterItem(QGraphicsItem *masterItem)
{
	mMaster = masterItem;
	mFrom = mMaster->boundingRect().topLeft();
	mFrom.setX(mFrom.x() + mMaster->boundingRect().width() / 2);
	mFrom.setY(mFrom.y() + 10);
	mTo = mFrom;
	mTo.setY(mTo.y() - 300);
	mLine.setLine(QLine(mFrom.toPoint(), mTo.toPoint()));
}

void Rotater::paint(QPainter *painter, const QStyleOptionGraphicsItem *style, QWidget *widget)
{
	Q_UNUSED(style)
	Q_UNUSED(widget)

	painter->save();

	QBrush brush;
	brush.setColor(Qt::blue);
	brush.setStyle(Qt::SolidPattern);

	painter->setBrush(brush);
	painter->setOpacity(0.5);
	painter->setPen(Qt::blue);
	painter->drawLine(mLine.line());

	painter->restore();
}

QRectF Rotater::boundingRect() const
{
	QRect rect = QRect(mLine.line().p1().toPoint(), mLine.line().p2().toPoint()).normalized().adjusted(-5, -5, 5, 5);
	return rect;
}

