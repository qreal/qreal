#pragma once

#include <interpreterBase/robotModel/commonRobotModel.h>

namespace trikKitInterpreter {
namespace robotModel {

class TrikRobotModelBase : public interpreterBase::robotModel::CommonRobotModel
{
	Q_OBJECT

public:
	explicit TrikRobotModelBase(QString const &kitId);

	QList<interpreterBase::robotModel::PortInfo> configurablePorts() const override;
	QList<interpreterBase::robotModel::DeviceInfo> convertibleBases() const override;

protected:
	virtual interpreterBase::robotModel::DeviceInfo displayInfo() const;
	virtual interpreterBase::robotModel::DeviceInfo speakerInfo() const;
	virtual interpreterBase::robotModel::DeviceInfo buttonInfo() const;

	virtual interpreterBase::robotModel::DeviceInfo powerMotorInfo() const;
	virtual interpreterBase::robotModel::DeviceInfo servoMotorInfo() const;
	virtual interpreterBase::robotModel::DeviceInfo encoderInfo() const;

	virtual interpreterBase::robotModel::DeviceInfo lightSensorInfo() const;
	virtual interpreterBase::robotModel::DeviceInfo infraredSensorInfo() const;

	virtual interpreterBase::robotModel::DeviceInfo sonarSensorInfo() const;

	virtual interpreterBase::robotModel::DeviceInfo motionSensorInfo() const;

	virtual interpreterBase::robotModel::DeviceInfo ledInfo() const;
	virtual interpreterBase::robotModel::DeviceInfo lineSensorInfo() const;
	virtual interpreterBase::robotModel::DeviceInfo colorSensorInfo() const;
	virtual interpreterBase::robotModel::DeviceInfo objectSensorInfo() const;
};

}
}
