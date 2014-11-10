#pragma once

#include <interpreterBase/blocksBase/common/deviceBlock.h>

#include "commonTwoDModel/robotModel/parts/marker.h"

namespace twoDModel {
namespace blocks {

/// An interpreter`s implementation for RemoveMarker block. See description in documentation.
class MarkerUpBlock : public interpreterBase::blocksBase::common::DeviceBlock<robotModel::parts::Marker>
{
	Q_OBJECT

public:
	explicit MarkerUpBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);

private:
	void doJob(robotModel::parts::Marker &marker) override;
};

}
}
