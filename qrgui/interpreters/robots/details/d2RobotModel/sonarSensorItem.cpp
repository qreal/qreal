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
	SensorItem::paint(painter, style, widget);
	painter->save();
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
	return mWorldModel.sonarScanningRegion(mConfiguration.position(mPort), mConfiguration.direction(mPort));
}
