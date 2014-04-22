#include "trikV5RobotModelBase.h"

using namespace trikKitInterpreter::robotModel;
using namespace interpreterBase::robotModel;

TrikV5RobotModelBase::TrikV5RobotModelBase()
{
}

QList<DeviceInfo> TrikV5RobotModelBase::convertibleBases() const
{
	return TrikV4RobotModelBase::convertibleBases();
}

DeviceInfo TrikV5RobotModelBase::movementSensorInfo() const
{
//	return DeviceInfo::create<robotParts::>
}
