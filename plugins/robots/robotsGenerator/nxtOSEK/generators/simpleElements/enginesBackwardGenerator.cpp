#include "enginesBackwardGenerator.h"
#include "../../nxtOSEKRobotGenerator.h"

using namespace robots::generator;

EnginesBackwardGenerator::EnginesBackwardGenerator()
{
}

QList<SmartLine> EnginesBackwardGenerator::convertElementIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
		, qReal::Id const elementId, qReal::Id const logicElementId)
{
	QList<SmartLine> result;

	QStringList const cmds = nxtGen->api()->stringProperty(logicElementId, "Power").split(";", QString::SkipEmptyParts);
	for (int i = 0; i < cmds.size() - 1; ++i) {
		result.append(SmartLine(cmds.at(i) + ";", elementId));
	}
	foreach (QString enginePort, portsToEngineNames(nxtGen->api()->stringProperty(logicElementId, "Ports"))) {
		result.append(SmartLine(
			"nxt_motor_set_speed(" + enginePort + ", -" + cmds.last() + ", 1);",
			elementId));
	}

	return result;
}
