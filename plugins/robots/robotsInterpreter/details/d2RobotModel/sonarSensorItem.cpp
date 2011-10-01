#include "sonarSensorItem.h"

using namespace qReal::interpreters::robots;
using namespace details::d2Model;

SonarSensorItem::SonarSensorItem(WorldModel const &worldModel
		, SensorsConfiguration &configuration, inputPort::InputPortEnum port)
	: SensorItem(configuration, port)
	, mWorldModel(worldModel)
{
	setFlags(ItemIsSelectable | ItemIsMovable | ItemSendsGeometryChanges);
}

void SonarSensorItem::drawItem(QPainter *painter, QStyleOptionGraphicsItem const *style, QWidget *widget)
{
	Q_UNUSED(style)
	Q_UNUSED(widget)

	QBrush brush;
	brush.setColor(color());
	brush.setStyle(Qt::SolidPattern);

	QRectF rect(-size, -size, size * 2, size * 2);

	painter->setBrush(brush);
	painter->setOpacity(0.5);
	painter->setPen(color());
	painter->drawEllipse(rect.adjusted(1, 1, -1, -1));

	painter->setOpacity(0.2);
	painter->setBrush(Qt::black);
	painter->drawPath(shape());
}

void SonarSensorItem::drawExtractionForItem(QPainter *painter)
{
	if (!isSelected())
		return;
	painter->setPen(QPen(Qt::black));
	painter->drawPath(shape());
}

QRectF SonarSensorItem::boundingRect() const
{
	return scanningRegion().boundingRect().adjusted(-5, -5, 5, 5);
}

QPainterPath SonarSensorItem::scanningRegion() const
{
	return mWorldModel.sonarScanningRegion(QPoint(), mBaseDir + mConfiguration.direction(mPort));
}

QPainterPath SonarSensorItem::shape() const
{
	QPainterPath path = scanningRegion();
	path.addEllipse(QRectF(-size, -size, size * 2, size * 2));
	return path;
}
