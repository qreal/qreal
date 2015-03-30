#pragma once

#include <kitBase/blocksBase/common/deviceBlock.h>

#include "twoDModel/robotModel/parts/marker.h"

namespace twoDModel {
namespace blocks {

/// An interpreter`s implementation for RemoveMarker block. See description in documentation.
class TWO_D_MODEL_EXPORT MarkerUpBlock
		: public kitBase::blocksBase::common::DeviceBlock<robotModel::parts::Marker>
{
	Q_OBJECT

public:
	explicit MarkerUpBlock(kitBase::robotModel::RobotModelInterface &robotModel);

private:
	void doJob(robotModel::parts::Marker &marker) override;
};

}
}
