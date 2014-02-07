#pragma once

#include <interpreterBase/robotModel/portInfo.h>
#include <interpreterBase/robotModel/robotParts/touchSensor.h>
#include <utils/robotCommunication/robotCommunicator.h>

namespace nxtKitInterpreter {
namespace robotModel {
namespace real {
namespace parts {

class TouchSensor : public interpreterBase::robotModel::robotParts::TouchSensor
{
	Q_OBJECT

public:
	TouchSensor(interpreterBase::robotModel::PluggableDeviceInfo const &info
			, interpreterBase::robotModel::PortInfo const &port
			, utils::robotCommunication::RobotCommunicator *robotCommunicator);

	void read() override;

protected:
	/// @todo
	virtual void sensorSpecificProcessResponse(QByteArray const &reading);
};

}
}
}
}
