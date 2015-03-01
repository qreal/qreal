#pragma once

#include "regionItem.h"

namespace twoDModel {
namespace items {

/// Ellipse-shaped zone in 2D model.
class EllipseRegion : public RegionItem
{
public:
	explicit EllipseRegion(QGraphicsItem *parent = 0);

private:
	QPainterPath shape() const override;
};

}
}
