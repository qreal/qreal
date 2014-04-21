#pragma once

#include <interpreterBase/robotModel/commonRobotModel.h>

namespace trikKitInterpreter {
namespace robotModel {

class TrikRobotModelBase : public interpreterBase::robotModel::CommonRobotModel
{
	Q_OBJECT

public:
	TrikRobotModelBase();

	QList<interpreterBase::robotModel::DeviceInfo> convertibleBases() const override;

protected:
	virtual interpreterBase::robotModel::DeviceInfo displayInfo() const;
	virtual interpreterBase::robotModel::DeviceInfo speakerInfo() const;
	virtual interpreterBase::robotModel::DeviceInfo buttonsInfo() const;
	virtual interpreterBase::robotModel::DeviceInfo motorInfo() const;
	virtual interpreterBase::robotModel::DeviceInfo encoderInfo() const;
	virtual interpreterBase::robotModel::DeviceInfo lightSensorInfo() const;
};

}
}
