#include "ev3RobotModelBase.h"

#include "parts/ev3Motor.h"
#include "parts/ev3Speaker.h"
#include "parts/ev3Display.h"

#include <interpreterBase/robotModel/robotParts/display.h>
#include <interpreterBase/robotModel/robotParts/speaker.h>
#include <interpreterBase/robotModel/robotParts/button.h>


using namespace ev3KitInterpreter::robotModel;
using namespace interpreterBase::robotModel;

Ev3RobotModelBase::Ev3RobotModelBase(QString const &kitId)
	: CommonRobotModel(kitId)
{
	addAllowedConnection(PortInfo("DisplayPort", output), { displayInfo() });
	addAllowedConnection(PortInfo("SpeakerPort", output), { speakerInfo() });

	addAllowedConnection(PortInfo("UpButtonPort", input, {}, "upButton"), { buttonInfo() });
	addAllowedConnection(PortInfo("DownButtonPort", input, {}, "downButton"), { buttonInfo() });
	addAllowedConnection(PortInfo("LeftButtonPort", input, {}, "leftButton"), { buttonInfo() });
	addAllowedConnection(PortInfo("RightButtonPort", input, {}, "rightButton"), { buttonInfo() });
	addAllowedConnection(PortInfo("BackButtonPort", input, {}, "backButton"), { buttonInfo() });
	addAllowedConnection(PortInfo("EnterButtonPort", input, {}, "enterButton"), { buttonInfo() });

	addAllowedConnection(PortInfo("A", output), { motorInfo() });
	addAllowedConnection(PortInfo("B", output), { motorInfo() });
	addAllowedConnection(PortInfo("C", output), { motorInfo() });
	addAllowedConnection(PortInfo("D", output), { motorInfo() });

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

DeviceInfo Ev3RobotModelBase::buttonInfo() const
{
	return DeviceInfo::create<robotParts::Button>();
}
