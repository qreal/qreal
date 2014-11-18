#pragma once

#include "commandConstants.h"
#include <QByteArray>

#include <interpreterBase/robotModel/robotParts/abstractSensor.h>
#include <utils/robotCommunication/robotCommunicator.h>

namespace ev3KitInterpreter {
namespace robotModel {
namespace real {
namespace parts {

class Ev3InputDevice : public QObject
{
    Q_OBJECT

public:

    Ev3InputDevice(utils::robotCommunication::RobotCommunicator &robotCommunicator
                   , interpreterBase::robotModel::PortInfo const &port
                   , SensorTypeEnum lowLevelSensorType
                   , INPUT_DEVICE_SUBCODE sensorMode);

    void send(QByteArray command, unsigned const responseSize, QByteArray reading);

    /// Returns a value of port that can be used as corresponding byte in request packages.
    char lowLevelPort() const;

    utils::robotCommunication::RobotCommunicator &mRobotCommunicator;
    char mLowLevelPort;
    SensorTypeEnum mLowLevelSensorType;
    INPUT_DEVICE_SUBCODE mSensorMode;
};

}
}
}
}
