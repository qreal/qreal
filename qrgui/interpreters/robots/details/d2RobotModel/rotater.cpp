#include "rotater.h"

#include <QtGui/QCursor>
#include <QDebug>

using namespace qReal::interpreters::robots;
using namespace details::d2Model;

Rotater::Rotater() : QGraphicsLineItem()
{
	setFlags(ItemIsSelectable | ItemIsMovable | ItemClipsChildrenToShape |
		ItemClipsToShape | ItemSendsGeometryChanges);

	setAcceptHoverEvents(true);
	setAcceptDrops(true);
	setCursor(QCursor(Qt::PointingHandCursor));
	setZValue(2);

	mFrom = QPointF(0,0);
	mTo = QPointF(0,0);
	setLine(QLine(mFrom.toPoint(), mTo.toPoint()));
	qDebug() << this->line();
}

void Rotater::setMasterItem(QGraphicsItem *masterItem)
{
	mMaster = masterItem;
	mFrom = mMaster->boundingRect().topLeft();
	mFrom.setX(mFrom.x() + mMaster->boundingRect().width() / 2);
	mFrom.setY(mFrom.y() + 10);
	mTo = mFrom;
	mTo.setY(mTo.y() - 300);
	setLine(QLine(mFrom.toPoint(), mTo.toPoint()));
	qDebug() << this->line();

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
	painter->drawLine(line());
	qDebug() << "draw" << line();

	painter->restore();


}

QRectF Rotater::boundingRect() const
{
	QRect rect = QRect(line().p1().toPoint(), line().p2().toPoint()).adjusted(-5, -5, 5, 5);
//	qDebug() << "rect:" << rect;
	return rect;
}

