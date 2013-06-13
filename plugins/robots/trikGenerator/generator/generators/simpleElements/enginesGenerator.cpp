#include "enginesGenerator.h"
#include "../../trikRobotGenerator.h"
#include "../listsmartline.h"

using namespace robots::trikGenerator;

EnginesGenerator::EnginesGenerator(QString const &engineType): mEngineType(engineType)
{
}

void EnginesGenerator::addInitAndTerminateCode(TrikRobotGenerator *nxtGen  // duplicated code - not true
		, qReal::Id const elementId, QString const &enginePort)
{
	QString const initTerminateVelocity = "0";
	QString const initTerminateBrakeMode = "1";
	QString const initCodeEngines = "nxt_motor_set_speed("
			+ enginePort + ", "
			+ initTerminateVelocity + ", "
			+ initTerminateBrakeMode + ");";
	QString const terminateCodeEngines = "nxt_motor_set_speed("
			+ enginePort + ", "
			+ initTerminateVelocity + ", "
			+ initTerminateBrakeMode + ");";
	if (!ListSmartLine::isContains(nxtGen->initCode(), initCodeEngines)) {
		nxtGen->initCode().append(SmartLine(initCodeEngines, elementId));
		nxtGen->terminateCode().append(SmartLine(terminateCodeEngines, elementId));
	}
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
	QByteArray const brakeMode = brakeModeRaw == QString::fromUtf8("скользить") ? "0" : "1";

	foreach (QString const &enginePort, portsToEngineNames(nxtGen->api()->stringProperty(logicElementId, "Ports"))) {
		result.append(
				SmartLine("brick.motor("
						+ enginePort + ").setPower("
						+ intPower + ")"
				, elementId));
		addInitAndTerminateCode(nxtGen, elementId, enginePort);
	}

	return result;
}
