#pragma once

#include <interpreterBase/robotModel/robotParts/colorSensor.h>
#include <utils/robotCommunication/robotCommunicator.h>

#include "commandConstants.h"
#include "src/robotModel/real/parts/ev3InputDevice.h"

namespace ev3KitInterpreter {
namespace robotModel {
namespace real {
namespace parts {

class ColorSensorImpl : public QObject
{
    Q_OBJECT

public:
    ColorSensorImpl(interpreterBase::robotModel::PortInfo const &port
              , utils::robotCommunication::RobotCommunicator &robotCommunicator
              , SensorTypeEnum lowLevelType);

    void read();

signals:
    void newData(int reading);

private:
    Ev3InputDevice mImplementation;
    utils::robotCommunication::RobotCommunicator &mRobotCommunicator;
    SensorTypeEnum mLowLevelType;
};

}
}
}
}
