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
}

void Ev3InputDevice::send(QByteArray command, unsigned const responseSize, QByteArray reading)
{
    mRobotCommunicator.send(command, responseSize, reading);
}

char Ev3InputDevice::lowLevelPort() const
{
    return mLowLevelPort;
}
