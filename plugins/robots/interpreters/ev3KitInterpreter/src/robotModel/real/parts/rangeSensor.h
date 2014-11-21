#pragma once

#include <interpreterBase/robotModel/robotParts/rangeSensor.h>
#include <utils/robotCommunication/robotCommunicator.h>

#include "commandConstants.h"
#include "ev3InputDevice.h"

namespace ev3KitInterpreter {
namespace robotModel {
namespace real {
namespace parts {

class RangeSensor : public interpreterBase::robotModel::robotParts::RangeSensor
{
    Q_OBJECT

public:
    RangeSensor(interpreterBase::robotModel::DeviceInfo const &info
              , interpreterBase::robotModel::PortInfo const &port
              , utils::robotCommunication::RobotCommunicator &robotCommunicator);

    void read() override;

private:
    Ev3InputDevice mImplementation;
    utils::robotCommunication::RobotCommunicator &mRobotCommunicator;
};

}
}
}
}
