#pragma once

#include <kitBase/blocksBase/common/deviceBlock.h>
#include <trikKit/robotModel/parts/trikLineSensor.h>

namespace trik {
namespace blocks {
namespace details {

/// Interpreter implementation for "Initialize Videocamera" block.
class InitCameraBlock : public kitBase::blocksBase::common::DeviceBlock<trik::robotModel::parts::TrikLineSensor>
{
public:
	InitCameraBlock(kitBase::robotModel::RobotModelInterface &robotModel);

protected:
	virtual void doJob(robotModel::parts::TrikLineSensor &camera);
};

}
}
}
