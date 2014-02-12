#pragma once

#include <interpreterBase/robotModel/commonRobotModel.h>

namespace nxtKitInterpreter {
namespace robotModel {

class NxtRobotModelBase : public interpreterBase::robotModel::CommonRobotModel
{
	Q_OBJECT

public:
	NxtRobotModelBase();

	QList<interpreterBase::robotModel::PortInfo> configurablePorts() const override;

	QList<interpreterBase::robotModel::PluggableDeviceInfo> convertibleBases() const override;

protected:
	virtual interpreterBase::robotModel::PluggableDeviceInfo touchSensorInfo() const;
	virtual interpreterBase::robotModel::PluggableDeviceInfo sonarSensorInfo() const;
	virtual interpreterBase::robotModel::PluggableDeviceInfo lightSensorInfo() const;
	virtual interpreterBase::robotModel::PluggableDeviceInfo colorSensorInfo() const;
	virtual interpreterBase::robotModel::PluggableDeviceInfo soundSensorInfo() const;
	virtual interpreterBase::robotModel::PluggableDeviceInfo gyroscopeSensorInfo() const;
	virtual interpreterBase::robotModel::PluggableDeviceInfo accelerometerSensorInfo() const;
};

}
}
