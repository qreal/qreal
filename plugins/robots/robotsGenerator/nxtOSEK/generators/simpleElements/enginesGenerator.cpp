#include "enginesGenerator.h"
#include "../../nxtOSEKRobotGenerator.h"
#include "../listsmartline.h"

using namespace robots::generator;

EnginesGenerator::EnginesGenerator(QString const &engineType): mEngineType(engineType)
{
}

void EnginesGenerator::addInitAndTerminateCode(NxtOSEKRobotGenerator *nxtGen  // duplicated code - not true
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

QList<SmartLine> EnginesGenerator::convertElementIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
		 , qReal::Id const elementId, qReal::Id const logicElementId)
{
	QList<SmartLine> result;
	QString const power = nxtGen->api()->stringProperty(logicElementId, "Power");
	QString brakeMode = nxtGen->api()->stringProperty(logicElementId, "BrakeMode");
	if (brakeMode.compare("скользить")) {
		brakeMode = "0";
	} else if (brakeMode.compare("тормозить")){
		brakeMode = "1";
	} else {
		brakeMode = "ololo-trollolo";
	}

	QString signRotate = "";
	if (mEngineType == "EnginesBackward") {
		signRotate = "-";
	}

	foreach (QString const &enginePort, portsToEngineNames(nxtGen->api()->stringProperty(logicElementId, "Ports"))) {

		result.append(SmartLine("nxt_motor_set_speed(NXT_PORT_"
				+ enginePort + ", "
				+ signRotate + power + ", "
				+ brakeMode + ");", elementId));
		addInitAndTerminateCode(nxtGen, elementId, "NXT_PORT_" + enginePort);
	}

	return result;
}
