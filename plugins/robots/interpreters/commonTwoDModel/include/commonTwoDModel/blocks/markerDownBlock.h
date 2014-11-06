#pragma once

#include <interpreterBase/blocksBase/common/deviceBlock.h>

#include "commonTwoDModel/robotModel/parts/marker.h"

namespace twoDModel {
namespace blocks {

/// An interpreter`s implementation for PrepareMarker block. See description in documentation.
class COMMON_TWO_D_MODEL_EXPORT MarkerDownBlock : public interpreterBase::blocksBase::common::DeviceBlock<robotModel::parts::Marker>
{
	Q_OBJECT

public:
	explicit MarkerDownBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);

private:
	void doJob(robotModel::parts::Marker &marker) override;
};

}
}
