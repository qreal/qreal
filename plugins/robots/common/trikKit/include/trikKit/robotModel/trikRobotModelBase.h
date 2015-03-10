#pragma once

#include <kitBase/robotModel/commonRobotModel.h>

namespace trik {
namespace robotModel {

class TrikRobotModelBase : public kitBase::robotModel::CommonRobotModel
{
	Q_OBJECT

public:
	TrikRobotModelBase(const QString &kitId, const QString &robotId);

	QList<kitBase::robotModel::PortInfo> configurablePorts() const override;
	QList<kitBase::robotModel::DeviceInfo> convertibleBases() const override;

protected:
	virtual kitBase::robotModel::DeviceInfo displayInfo() const;
	virtual kitBase::robotModel::DeviceInfo speakerInfo() const;
	virtual kitBase::robotModel::DeviceInfo buttonInfo() const;

	virtual kitBase::robotModel::DeviceInfo powerMotorInfo() const;
	virtual kitBase::robotModel::DeviceInfo servoMotorInfo() const;
	virtual kitBase::robotModel::DeviceInfo encoderInfo() const;

	virtual kitBase::robotModel::DeviceInfo lightSensorInfo() const;
	virtual kitBase::robotModel::DeviceInfo infraredSensorInfo() const;

	virtual kitBase::robotModel::DeviceInfo sonarSensorInfo() const;

	virtual kitBase::robotModel::DeviceInfo motionSensorInfo() const;

	virtual kitBase::robotModel::DeviceInfo gyroscopeInfo() const;
	virtual kitBase::robotModel::DeviceInfo accelerometerInfo() const;

	virtual kitBase::robotModel::DeviceInfo ledInfo() const;
	virtual kitBase::robotModel::DeviceInfo lineSensorInfo() const;
	virtual kitBase::robotModel::DeviceInfo colorSensorInfo() const;
	virtual kitBase::robotModel::DeviceInfo objectSensorInfo() const;
	virtual kitBase::robotModel::DeviceInfo shellInfo() const;

	virtual kitBase::robotModel::DeviceInfo gamepadButtonInfo() const;
	virtual kitBase::robotModel::DeviceInfo gamepadPadInfo() const;
	virtual kitBase::robotModel::DeviceInfo gamepadPadPressSensorInfo() const;
	virtual kitBase::robotModel::DeviceInfo gamepadWheelInfo() const;
	virtual kitBase::robotModel::DeviceInfo gamepadConnectionIndicatorInfo() const;
};

}
}
