#include "enginesForwardGenerator.h"
#include "../../nxtOSEKRobotGenerator.h"

using namespace robots::generator;

EnginesForwardGenerator::EnginesForwardGenerator()
{
}

QList<SmartLine> EnginesForwardGenerator::convertElementIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
		, qReal::Id const elementId, qReal::Id const logicElementId)
{
	QList<SmartLine> result;
	foreach (QString enginePort, portsToEngineNames(nxtGen->api()->stringProperty(logicElementId, "Ports"))) {
		result.append(SmartLine("nxt_motor_set_speed(" + enginePort + ", "
				+ nxtGen->api()->stringProperty(logicElementId, "Power") + ", 1);", elementId));
	}

	return result;
}
