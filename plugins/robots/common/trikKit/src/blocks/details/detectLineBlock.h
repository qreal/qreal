#pragma once

#include <kitBase/blocksBase/common/deviceBlock.h>
#include <trikKit/robotModel/parts/trikLineSensor.h>

namespace trik {
namespace blocks {
namespace details {

class DetectLineBlock : public kitBase::blocksBase::common::DeviceBlock<trik::robotModel::parts::TrikLineSensor>
{
public:
	DetectLineBlock(kitBase::robotModel::RobotModelInterface &robotModel);

protected:
	virtual void doJob(robotModel::parts::TrikLineSensor &camera);
};

}
}
}
