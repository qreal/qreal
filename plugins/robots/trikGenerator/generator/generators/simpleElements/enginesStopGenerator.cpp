#include "enginesStopGenerator.h"
#include "../../trikRobotGenerator.h"

using namespace robots::trikGenerator;

EnginesStopGenerator::EnginesStopGenerator()
{
}

QList<SmartLine> EnginesStopGenerator::convertElementIntoDirectCommand(TrikRobotGenerator *generator
		, qReal::Id const elementId, qReal::Id const logicElementId)
{
	QList<SmartLine> result;
	foreach (QString enginePort, portsToEngineNames(generator->api()->stringProperty(logicElementId, "Ports"))) {
		result.append(
				SmartLine("brick.powerMotor("
						+ enginePort + ").powerOff();"
				, elementId));
	}

	return result;
}
