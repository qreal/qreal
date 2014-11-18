#include "ev3InputDevice.h"

#include <utils/tracer.h>

using namespace ev3KitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;
using namespace utils;

Ev3InputDevice::Ev3InputDevice(
        utils::robotCommunication::RobotCommunicator &robotCommunicator
       , interpreterBase::robotModel::PortInfo const &port
       , SensorTypeEnum lowLevelSensorType
       , INPUT_DEVICE_SUBCODE sensorMode)
    : mRobotCommunicator(robotCommunicator)
    , mLowLevelPort(port.name().at(0).toLatin1() - '1')
    , mLowLevelSensorType(lowLevelSensorType)
    , mSensorMode(sensorMode)
{
//    QObject::connect(&mRobotCommunicator, &utils::robotCommunication::RobotCommunicator::response
//            , this, &Ev3InputDevice::readingDone);
}

//void Ev3InputDevice::readingDone(QObject *addressee, QByteArray const &reading)
//{
//    if (addressee == this) {
//        processResponse(reading);
//    }
//}

void Ev3InputDevice::send(QByteArray command, unsigned const responseSize, QByteArray reading)
{
    mRobotCommunicator.send(command, responseSize, reading);
}

//void Ev3InputDevice::processResponse(QByteArray const &reading)
//{
//    if (reading.isEmpty()) {
//        mState = idle;
//    }
//    else {
//        //emit configured(true);
//    }
//}

char Ev3InputDevice::lowLevelPort() const
{
    return mLowLevelPort;
}

//Ev3InputDevice::State Ev3InputDevice::state() const
//{
//    return mState;
//}

//void Ev3InputDevice::setState(Ev3InputDevice::State state)
//{
//    mState = state;
//}


