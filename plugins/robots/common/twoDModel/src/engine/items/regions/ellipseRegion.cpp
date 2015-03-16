#include "ellipseRegion.h"

using namespace twoDModel::items;

EllipseRegion::EllipseRegion(QGraphicsItem *parent)
	: RegionItem(parent)
{
}

QPainterPath EllipseRegion::shape() const
{
	QPainterPath result;
	result.addEllipse(boundingRect());
	return result;
}

QString EllipseRegion::regionType() const
{
	return "ellipse";
}
