#pragma once

#include <kitBase/robotModel/commonRobotModel.h>

namespace ev3 {
namespace robotModel {

class Ev3RobotModelBase : public kitBase::robotModel::CommonRobotModel
{
	Q_OBJECT

public:
	Ev3RobotModelBase(const QString &kitId, const QString &robotId);

	QList<kitBase::robotModel::DeviceInfo> convertibleBases() const override;

protected:
	virtual kitBase::robotModel::DeviceInfo motorInfo() const;
	virtual kitBase::robotModel::DeviceInfo speakerInfo() const;
	virtual kitBase::robotModel::DeviceInfo buttonInfo() const;
	virtual kitBase::robotModel::DeviceInfo displayInfo() const;
	virtual kitBase::robotModel::DeviceInfo touchSensorInfo() const;
	virtual kitBase::robotModel::DeviceInfo lightSensorInfo() const;
	virtual kitBase::robotModel::DeviceInfo rangeSensorInfo() const;
	virtual kitBase::robotModel::DeviceInfo colorFullSensorInfo() const;
};

}
}
