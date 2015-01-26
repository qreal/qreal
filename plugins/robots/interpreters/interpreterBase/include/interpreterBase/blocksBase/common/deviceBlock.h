#pragma once

#include "interpreterBase/blocksBase/robotsBlock.h"
#include "interpreterBase/robotModel/robotModelInterface.h"
#include "interpreterBase/robotModel/robotModelUtils.h"

namespace interpreterBase {
namespace blocksBase {
namespace common {

/// A base for all blocks that work with some robot`s device. This block will
/// perform search of some device of the given type and call doJob() from it if it was found
/// or display an error otherwise.
template<typename Device>
class DeviceBlock : public RobotsBlock
{
public:
	explicit DeviceBlock(interpreterBase::robotModel::RobotModelInterface &robotModel)
		: mRobotModel(robotModel)
	{
	}

	void run() override
	{
		robotModel::DeviceInfo const deviceInfo = robotModel::DeviceInfo::create<Device>();
		QString const port = deviceInfo.name()[0].toUpper() + deviceInfo.name().mid(1) + "Port";
		Device * const device = robotModel::RobotModelUtils::findDevice<Device>(mRobotModel, port);
		if (device) {
			doJob(*device);
		} else {
			error(QObject::tr("%1 is not configured.").arg(deviceInfo.friendlyName()));
		}
	}

protected:
	/// Implementation may consider that the device is configured and ready to work.
	virtual void doJob(Device &display) = 0;

private:
	interpreterBase::robotModel::RobotModelInterface &mRobotModel;
};

}
}
}
