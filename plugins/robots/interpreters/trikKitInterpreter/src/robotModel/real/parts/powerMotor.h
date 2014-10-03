#pragma once

#include "robotModel/parts/trikPowerMotor.h"

namespace trikKitInterpreter {
namespace robotModel {
namespace real {
namespace parts {

class PowerMotor : public robotModel::parts::TrikPowerMotor
{
	Q_OBJECT

public:
	PowerMotor(interpreterBase::robotModel::DeviceInfo const &info
			, interpreterBase::robotModel::PortInfo const &port);
};

}
}
}
}
