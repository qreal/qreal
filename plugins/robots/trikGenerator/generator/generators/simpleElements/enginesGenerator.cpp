#include "enginesGenerator.h"
#include "../../trikRobotGenerator.h"
#include "../listsmartline.h"

using namespace robots::trikGenerator;

EnginesGenerator::EnginesGenerator(QString const &engineType): mEngineType(engineType)
{
}

QList<SmartLine> EnginesGenerator::convertElementIntoDirectCommand(TrikRobotGenerator *generator
		 , qReal::Id const elementId, qReal::Id const logicElementId)
{
	QList<SmartLine> result;

	QString const signRotate = mEngineType == "EnginesBackward" ? "-" : "";
	QString const power = generator->api()->stringProperty(logicElementId, "Power");
	QString const signedPower = signRotate + power;
	QString const intPower = generator->variables().expressionToInt(signedPower);

	QString const brakeModeRaw = generator->api()->stringProperty(logicElementId, "Mode");

	foreach (QString const &enginePort, portsToEngineNames(generator->api()->stringProperty(logicElementId, "Ports"))) {
		result.append(
				SmartLine("brick.powerMotor("
						+ enginePort + ").setPower("
						+ intPower + ");"
				, elementId));
	}

	return result;
}
