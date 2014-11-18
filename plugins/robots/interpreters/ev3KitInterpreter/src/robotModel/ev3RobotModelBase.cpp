#include "ev3RobotModelBase.h"

#include "parts/ev3Motor.h"
#include "parts/ev3Speaker.h"
#include "parts/ev3Display.h"

#include <interpreterBase/robotModel/robotParts/display.h>
#include <interpreterBase/robotModel/robotParts/speaker.h>
#include <interpreterBase/robotModel/robotParts/touchSensor.h>

using namespace ev3KitInterpreter::robotModel;
using namespace interpreterBase::robotModel;

Ev3RobotModelBase::Ev3RobotModelBase(QString const &kitId, QString const &robotId)
    : CommonRobotModel(kitId, robotId)
{
    QList<DeviceInfo> const inputPortConnections = {
            touchSensorInfo()
    };

    addAllowedConnection(PortInfo("DisplayPort", output), { displayInfo() });
    addAllowedConnection(PortInfo("SpeakerPort", output), { speakerInfo() });

    addAllowedConnection(PortInfo("A", output), { motorInfo() });
    addAllowedConnection(PortInfo("B", output), { motorInfo() });
    addAllowedConnection(PortInfo("C", output), { motorInfo() });
    addAllowedConnection(PortInfo("D", output), { motorInfo() });

    addAllowedConnection(PortInfo("1", input, {}, "sensor1"), inputPortConnections);
    addAllowedConnection(PortInfo("2", input, {}, "sensor2"), inputPortConnections);
    addAllowedConnection(PortInfo("3", input, {}, "sensor3"), inputPortConnections);
    addAllowedConnection(PortInfo("4", input, {}, "sensor4"), inputPortConnections);

}

QList<DeviceInfo> Ev3RobotModelBase::convertibleBases() const
{
    return { DeviceInfo::create<robotParts::TouchSensor>()
    };
}

DeviceInfo Ev3RobotModelBase::motorInfo() const
{
    return DeviceInfo::create<ev3KitInterpreter::robotModel::parts::Ev3Motor>();
}

DeviceInfo Ev3RobotModelBase::displayInfo() const
{
    return DeviceInfo::create<ev3KitInterpreter::robotModel::parts::Ev3Display>();
}

DeviceInfo Ev3RobotModelBase::speakerInfo() const
{
    return DeviceInfo::create<ev3KitInterpreter::robotModel::parts::Ev3Speaker>();
}

DeviceInfo Ev3RobotModelBase::touchSensorInfo() const
{
    return DeviceInfo::create<robotParts::TouchSensor>();
}
