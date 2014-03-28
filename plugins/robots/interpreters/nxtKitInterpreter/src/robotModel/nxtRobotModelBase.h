#pragma once

#include <interpreterBase/robotModel/commonRobotModel.h>

namespace nxtKitInterpreter {
namespace robotModel {

class NxtRobotModelBase : public interpreterBase::robotModel::CommonRobotModel
{
	Q_OBJECT

public:
	NxtRobotModelBase();

	QList<interpreterBase::robotModel::DeviceInfo> convertibleBases() const override;

protected:
	virtual interpreterBase::robotModel::DeviceInfo displayInfo() const;
	virtual interpreterBase::robotModel::DeviceInfo speakerInfo() const;
	virtual interpreterBase::robotModel::DeviceInfo buttonsInfo() const;
	virtual interpreterBase::robotModel::DeviceInfo motorInfo() const;
	virtual interpreterBase::robotModel::DeviceInfo encoderInfo() const;
	virtual interpreterBase::robotModel::DeviceInfo touchSensorInfo() const;
	virtual interpreterBase::robotModel::DeviceInfo sonarSensorInfo() const;
	virtual interpreterBase::robotModel::DeviceInfo lightSensorInfo() const;
	virtual interpreterBase::robotModel::DeviceInfo colorSensorInfo() const;
	virtual interpreterBase::robotModel::DeviceInfo soundSensorInfo() const;
	virtual interpreterBase::robotModel::DeviceInfo gyroscopeSensorInfo() const;
	virtual interpreterBase::robotModel::DeviceInfo accelerometerSensorInfo() const;
};

}
}
