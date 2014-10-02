#include "ev3RobotModelBase.h"

#include "parts/ev3Motor.h"
#include "parts/ev3Speaker.h"
#include "parts/ev3Display.h"


using namespace ev3KitInterpreter::robotModel;
using namespace interpreterBase::robotModel;

Ev3RobotModelBase::Ev3RobotModelBase(QString const &kitId)
	: CommonRobotModel(kitId)
{
	addAllowedConnection(PortInfo("A", output), { motorInfo() });
	addAllowedConnection(PortInfo("B", output), { motorInfo() });
	addAllowedConnection(PortInfo("C", output), { motorInfo() });

}

DeviceInfo Ev3RobotModelBase::motorInfo() const
{
	return DeviceInfo::create<ev3KitInterpreter::robotModel::parts::Ev3Motor>();
}


DeviceInfo Ev3RobotModelBase::speakerInfo() const
{
	return DeviceInfo::create<ev3KitInterpreter::robotModel::parts::Ev3Speaker>();
}

DeviceInfo Ev3RobotModelBase::displayInfo() const
{
	return DeviceInfo::create<ev3KitInterpreter::robotModel::parts::Ev3Display>();
}
