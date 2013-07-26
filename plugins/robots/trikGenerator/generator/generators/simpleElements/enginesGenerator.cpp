#include "enginesGenerator.h"
#include "../../trikRobotGenerator.h"
#include "../listsmartline.h"

using namespace robots::trikGenerator;

EnginesGenerator::EnginesGenerator(QString const &engineType): mEngineType(engineType)
{
}

QList<SmartLine> EnginesGenerator::convertElementIntoDirectCommand(TrikRobotGenerator *nxtGen
		 , qReal::Id const elementId, qReal::Id const logicElementId)
{
	QList<SmartLine> result;

	QString const signRotate = mEngineType == "EnginesBackward" ? "-" : "";
	QString const power = nxtGen->api()->stringProperty(logicElementId, "Power");
	QString const signedPower = signRotate + power;
	QString const intPower = nxtGen->variables().expressionToInt(signedPower);

	QString const brakeModeRaw = nxtGen->api()->stringProperty(logicElementId, "Mode");

	foreach (QString const &enginePort, portsToEngineNames(nxtGen->api()->stringProperty(logicElementId, "Ports"))) {
		result.append(
				SmartLine("brick.motor("
						+ enginePort + ").setPower("
						+ intPower + ");"
				, elementId));
	}

	return result;
}
