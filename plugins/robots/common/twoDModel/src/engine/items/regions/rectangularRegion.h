#pragma once

#include "regionItem.h"

namespace twoDModel {
namespace items {

/// Rectangle-shaped zone in 2D model.
class RectangularRegion : public RegionItem
{
public:
	explicit RectangularRegion(QGraphicsItem *parent = 0);

private:
	QPainterPath shape() const override;
	QString regionType() const override;
};

}
}
