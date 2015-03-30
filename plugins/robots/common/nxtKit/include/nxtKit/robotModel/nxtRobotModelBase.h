#pragma once

#include <kitBase/robotModel/commonRobotModel.h>

namespace nxt {
namespace robotModel {

class NxtRobotModelBase : public kitBase::robotModel::CommonRobotModel
{
	Q_OBJECT

public:
	NxtRobotModelBase(const QString &kitId, const QString &robotId);

	QList<kitBase::robotModel::DeviceInfo> convertibleBases() const override;

protected:
	virtual kitBase::robotModel::DeviceInfo displayInfo() const;
	virtual kitBase::robotModel::DeviceInfo speakerInfo() const;
	virtual kitBase::robotModel::DeviceInfo buttonInfo() const;
	virtual kitBase::robotModel::DeviceInfo motorInfo() const;
	virtual kitBase::robotModel::DeviceInfo encoderInfo() const;
	virtual kitBase::robotModel::DeviceInfo touchSensorInfo() const;
	virtual kitBase::robotModel::DeviceInfo sonarSensorInfo() const;
	virtual kitBase::robotModel::DeviceInfo lightSensorInfo() const;
	virtual kitBase::robotModel::DeviceInfo colorFullSensorInfo() const;
	virtual kitBase::robotModel::DeviceInfo colorRedSensorInfo() const;
	virtual kitBase::robotModel::DeviceInfo colorGreenSensorInfo() const;
	virtual kitBase::robotModel::DeviceInfo colorBlueSensorInfo() const;
	virtual kitBase::robotModel::DeviceInfo colorPassiveSensorInfo() const;
	virtual kitBase::robotModel::DeviceInfo soundSensorInfo() const;
	virtual kitBase::robotModel::DeviceInfo gyroscopeSensorInfo() const;
	virtual kitBase::robotModel::DeviceInfo accelerometerSensorInfo() const;
};

}
}
