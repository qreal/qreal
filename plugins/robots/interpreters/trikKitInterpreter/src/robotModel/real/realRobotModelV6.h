#pragma once

#include "realRobotModelBase.h"

namespace trikKitInterpreter {
namespace robotModel {
namespace real {

class RealRobotModelV6 : public RealRobotModelBase
{
	Q_OBJECT

public:
	RealRobotModelV6(QString const &kitId);

	QString name() const override;
	QString friendlyName() const override;

private:
	interpreterBase::robotModel::DeviceInfo movementSensorInfo() const;

	interpreterBase::robotModel::robotParts::Device *createDevice(
			interpreterBase::robotModel::PortInfo const &port
			, interpreterBase::robotModel::DeviceInfo const &deviceInfo) override;
};

}
}
}
