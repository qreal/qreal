#include "sonarSensorItem.h"

using namespace twoDModel;
using namespace interpreterBase::robotModel;

SonarSensorItem::SonarSensorItem(WorldModel const &worldModel
		, SensorsConfiguration &configuration
		, PortInfo const &port)
	: SensorItem(configuration, port)
	, mWorldModel(worldModel)
	, mIcon(":/icons/sensors/2d_sonar.png")
{
	setFlags(ItemIsSelectable | ItemIsMovable | ItemSendsGeometryChanges);
}

void SonarSensorItem::drawItem(QPainter *painter, QStyleOptionGraphicsItem const *style, QWidget *widget)
{
	Q_UNUSED(style)
	Q_UNUSED(widget)

	painter->save();
	painter->setOpacity(0.2);
	painter->setBrush(Qt::black);
	painter->drawPath(scanningRegion());
	painter->restore();

	SensorItem::drawItem(painter, style, widget);
}

void SonarSensorItem::drawExtractionForItem(QPainter *painter)
{
	if (!isSelected()) {
		return;
	}

	SensorItem::drawExtractionForItem(painter);

	painter->save();
	QPen const pen = QPen(Qt::black);
	painter->setPen(pen);
	painter->setOpacity(0.7);
	painter->setRenderHints(painter->renderHints() | QPainter::Antialiasing);
	painter->drawPath(shape());
	painter->restore();
}

QRectF SonarSensorItem::boundingRect() const
{
	return scanningRegion().boundingRect().adjusted(-25, -15, 25, 15);
}

QPainterPath SonarSensorItem::scanningRegion() const
{
	return mWorldModel.sonarScanningRegion(QPointF());
}

QPainterPath SonarSensorItem::shape() const
{
	QPainterPath result = scanningRegion();
	result.addRect(mBoundingRect);
	return result;
}
