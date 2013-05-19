#include "sonarSensorItem.h"

using namespace qReal::interpreters::robots;
using namespace details::d2Model;

SonarSensorItem::SonarSensorItem(WorldModel const &worldModel
		, SensorsConfiguration &configuration, inputPort::InputPortEnum port)
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
	return scanningRegion().boundingRect().adjusted(-20, -10, 20, 10);
}

QPainterPath SonarSensorItem::scanningRegion() const
{
	return mWorldModel.sonarScanningRegion(QPoint(), mBaseDir + mConfiguration.direction(mPort));
}

QPainterPath SonarSensorItem::shape() const
{
	QPainterPath result = scanningRegion();
	result.addRect(mBoundingRect);
	return result;
}
