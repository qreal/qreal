#include "enginesStopGenerator.h"
#include "../../nxtOSEKRobotGenerator.h"

using namespace robots::generator;

EnginesStopGenerator::EnginesStopGenerator()
{
}

QList<SmartLine_old> EnginesStopGenerator::convertElementIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
		, qReal::Id const &elementId, qReal::Id const &logicElementId)
{
	QList<SmartLine_old> result;
	foreach (QString enginePort, portsToEngineNames(nxtGen->api()->stringProperty(logicElementId, "Ports"))) {
		result.append(SmartLine_old("nxt_motor_set_speed(" + enginePort + ", 0, 1);", elementId));
	}

	return result;
}
