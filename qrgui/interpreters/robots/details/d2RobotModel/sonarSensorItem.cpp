#include "sonarSensorItem.h"

#include <QtCore/QDebug>

using namespace qReal::interpreters::robots;
using namespace details::d2Model;

SonarSensorItem::SonarSensorItem(WorldModel const &worldModel
		, SensorsConfiguration &configuration, inputPort::InputPortEnum port)
	: SensorItem(configuration, port)
	, mWorldModel(worldModel)
{
	setFlags(ItemIsSelectable | ItemIsMovable | ItemSendsGeometryChanges);
}

void SonarSensorItem::paint(QPainter *painter, QStyleOptionGraphicsItem const *style, QWidget *widget)
{
	Q_UNUSED(style)
	Q_UNUSED(widget)

	painter->save();

	QBrush brush;
	brush.setColor(color());
	brush.setStyle(Qt::SolidPattern);

	painter->setBrush(brush);
	painter->setOpacity(0.5);
	painter->setPen(color());
	painter->drawEllipse(QRect(-size, -size, size * 2, size * 2));

	painter->setOpacity(0.2);
	painter->setBrush(Qt::black);
	painter->drawPath(scanningRegion());
	painter->restore();
}

QRectF SonarSensorItem::boundingRect() const
{
	return scanningRegion().boundingRect().adjusted(-5, -5, 5, 5);
}

QPainterPath SonarSensorItem::scanningRegion() const
{
	return mWorldModel.sonarScanningRegion(QPoint(), mConfiguration.direction(mPort));
}
