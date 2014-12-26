#pragma once

#include <interpreterBase/robotModel/commonRobotModel.h>

namespace nxtKitInterpreter {
namespace robotModel {

class NxtRobotModelBase : public interpreterBase::robotModel::CommonRobotModel
{
	Q_OBJECT

public:
	NxtRobotModelBase(QString const &kitId, QString const &robotId);

	QList<interpreterBase::robotModel::DeviceInfo> convertibleBases() const override;

protected:
	virtual interpreterBase::robotModel::DeviceInfo displayInfo() const;
	virtual interpreterBase::robotModel::DeviceInfo speakerInfo() const;
	virtual interpreterBase::robotModel::DeviceInfo buttonInfo() const;
	virtual interpreterBase::robotModel::DeviceInfo motorInfo() const;
	virtual interpreterBase::robotModel::DeviceInfo encoderInfo() const;
	virtual interpreterBase::robotModel::DeviceInfo touchSensorInfo() const;
	virtual interpreterBase::robotModel::DeviceInfo sonarSensorInfo() const;
	virtual interpreterBase::robotModel::DeviceInfo lightSensorInfo() const;
	virtual interpreterBase::robotModel::DeviceInfo colorFullSensorInfo() const;
	virtual interpreterBase::robotModel::DeviceInfo colorRedSensorInfo() const;
	virtual interpreterBase::robotModel::DeviceInfo colorGreenSensorInfo() const;
	virtual interpreterBase::robotModel::DeviceInfo colorBlueSensorInfo() const;
	virtual interpreterBase::robotModel::DeviceInfo colorPassiveSensorInfo() const;
	virtual interpreterBase::robotModel::DeviceInfo soundSensorInfo() const;
	virtual interpreterBase::robotModel::DeviceInfo gyroscopeSensorInfo() const;
	virtual interpreterBase::robotModel::DeviceInfo accelerometerSensorInfo() const;
};

}
}
