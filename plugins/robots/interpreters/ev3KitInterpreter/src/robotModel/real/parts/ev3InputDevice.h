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
//    enum State {
//        idle
//        , pending
//    };

    Ev3InputDevice(utils::robotCommunication::RobotCommunicator &robotCommunicator
                   , interpreterBase::robotModel::PortInfo const &port
                   , SensorTypeEnum lowLevelSensorType
                   , INPUT_DEVICE_SUBCODE sensorMode);

    void send(QByteArray command, unsigned const responseSize, QByteArray reading);

////    /// Returns a state of the communication with the device.
//      State state() const;

////    /// Sets a state of the communication with the device.
//      void setState(State state);

    /// Returns a value of port that can be used as corresponding byte in request packages.
    char lowLevelPort() const;

//signals:
//    /// Emitted when responce buffer has unknown data and must be processed by the concrete device.
//    void sensorSpecificProcessResponse(QByteArray const &reading);

//    /// Emitted when the configuration process is done.
//    void configured(bool success);

//private slots:
//    void readingDone(QObject *addressee, QByteArray const &reading);

//private:
//    void processResponse(QByteArray const &reading);

    utils::robotCommunication::RobotCommunicator &mRobotCommunicator;
    char mLowLevelPort;
    SensorTypeEnum mLowLevelSensorType;
    INPUT_DEVICE_SUBCODE mSensorMode;
//    State mState;
//    bool mResetDone;
};

}
}
}
}
