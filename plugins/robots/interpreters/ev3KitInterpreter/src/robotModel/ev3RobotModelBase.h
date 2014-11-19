#pragma once

#include <interpreterBase/robotModel/commonRobotModel.h>

namespace ev3KitInterpreter {
namespace robotModel {

class Ev3RobotModelBase : public interpreterBase::robotModel::CommonRobotModel
{
    Q_OBJECT

public:
    Ev3RobotModelBase(QString const &kitId, QString const &robotId);

    QList<interpreterBase::robotModel::DeviceInfo> convertibleBases() const override;

protected:
    virtual interpreterBase::robotModel::DeviceInfo motorInfo() const;
    virtual interpreterBase::robotModel::DeviceInfo speakerInfo() const;
    virtual interpreterBase::robotModel::DeviceInfo displayInfo() const;
    virtual interpreterBase::robotModel::DeviceInfo touchSensorInfo() const;
    virtual interpreterBase::robotModel::DeviceInfo lightSensorInfo() const;
    virtual interpreterBase::robotModel::DeviceInfo rangeSensorInfo() const;
    virtual interpreterBase::robotModel::DeviceInfo colorFullSensorInfo() const;
};

}
}
