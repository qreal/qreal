#include "enginesStopGenerator.h"
#include "../../trikRobotGenerator.h"

using namespace robots::trikGenerator;

EnginesStopGenerator::EnginesStopGenerator()
{
}

QList<SmartLine> EnginesStopGenerator::convertElementIntoDirectCommand(TrikRobotGenerator *nxtGen
		, qReal::Id const elementId, qReal::Id const logicElementId)
{
	QList<SmartLine> result;
	foreach (QString enginePort, portsToEngineNames(nxtGen->api()->stringProperty(logicElementId, "Ports"))) {
		result.append(
				SmartLine("brick.motor("
						+ enginePort + ").stop();"
				, elementId));
	}

	return result;
}
