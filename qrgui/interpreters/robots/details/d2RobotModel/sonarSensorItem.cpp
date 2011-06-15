#include "sonarSensorItem.h"

#include <QtCore/QDebug>

using namespace qReal::interpreters::robots;
using namespace details::d2Model;

SonarSensorItem::SonarSensorItem(QColor const &color, WorldModel const &worldModel
		, SensorsConfiguration const &configuration, inputPort::InputPortEnum port)
	: SensorItem(color)
	, mWorldModel(worldModel)
	, mConfiguration(configuration)
	, mPort(port)
{
	setFlags(ItemIsSelectable | ItemIsMovable | ItemSendsGeometryChanges);
}

void SonarSensorItem::paint(QPainter *painter, QStyleOptionGraphicsItem const *style, QWidget *widget)
{
	SensorItem::paint(painter, style, widget);
	QPainterPath ray = mWorldModel.sonarScanningRegion(mConfiguration.position(mPort), mConfiguration.direction(mPort));
	painter->save();
	painter->setOpacity(0.2);
	painter->setBrush(Qt::black);
	painter->drawPath(ray);
	painter->restore();
}

QRectF SonarSensorItem::boundingRect() const
{
	QPainterPath ray = mWorldModel.sonarScanningRegion(mConfiguration.position(mPort), mConfiguration.direction(mPort));
	return ray.boundingRect().adjusted(-5, -5, 5, 5);
}

