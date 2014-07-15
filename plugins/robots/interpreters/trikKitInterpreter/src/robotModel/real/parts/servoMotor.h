#pragma once

#include "robotModel/parts/trikServoMotor.h"

namespace trikKitInterpreter {
namespace robotModel {
namespace real {
namespace parts {

class ServoMotor : public robotModel::parts::TrikServoMotor
{
	Q_OBJECT

public:
	ServoMotor(interpreterBase::robotModel::DeviceInfo const &info
				, interpreterBase::robotModel::PortInfo const &port);

	void on(int speed) override;
	void stop() override;
	void off() override;
};

}
}
}
}
