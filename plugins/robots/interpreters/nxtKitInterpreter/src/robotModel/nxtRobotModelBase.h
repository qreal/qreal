#pragma once

#include <interpreterBase/robotModel/commonRobotModel.h>

namespace nxtKitInterpreter {
namespace robotModel {

class NxtRobotModelBase : public interpreterBase::robotModel::CommonRobotModel
{
	Q_OBJECT

public:
	virtual QList<interpreterBase::robotModel::PortInfo> availablePorts() const override;
	virtual QList<interpreterBase::robotModel::PluggableDeviceInfo> supportedSensors() const override;
};

}
}
