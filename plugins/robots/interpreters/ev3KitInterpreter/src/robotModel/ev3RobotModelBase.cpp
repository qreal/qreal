#include "ev3RobotModelBase.h"

#include "parts/ev3Motor.h"


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
