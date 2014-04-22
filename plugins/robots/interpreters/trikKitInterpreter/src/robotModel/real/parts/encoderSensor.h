#pragma once

#include <interpreterBase/robotModel/robotParts/encoderSensor.h>

namespace trikKitInterpreter {
namespace robotModel {
namespace real {
namespace parts {

class EncoderSensor : public interpreterBase::robotModel::robotParts::EncoderSensor
{
	Q_OBJECT

public:
	EncoderSensor(interpreterBase::robotModel::DeviceInfo const &info
			, interpreterBase::robotModel::PortInfo const &port);

	void read() override;
	void nullificate() override;

private:
	void doConfiguration() override;
};

}
}
}
}
