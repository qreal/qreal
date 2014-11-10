#pragma once

#include <interpreterBase/robotModel/commonRobotModel.h>

namespace ev3KitInterpreter {
namespace robotModel {

class Ev3RobotModelBase : public interpreterBase::robotModel::CommonRobotModel
{
	Q_OBJECT

public:
	Ev3RobotModelBase(QString const &kitId, QString const &robotId);

protected:
	virtual interpreterBase::robotModel::DeviceInfo motorInfo() const;
	virtual interpreterBase::robotModel::DeviceInfo speakerInfo() const;
	virtual interpreterBase::robotModel::DeviceInfo displayInfo() const;
};

}
}
