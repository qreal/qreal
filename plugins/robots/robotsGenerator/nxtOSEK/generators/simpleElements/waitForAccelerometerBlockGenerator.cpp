#include "waitForAccelerometerBlockGenerator.h"
#include "../../nxtOSEKRobotGenerator.h"
#include "../listsmartline.h"

using namespace robots::generator;

WaitForAccelerometerBlockGenerator::WaitForAccelerometerBlockGenerator()
{
}

void WaitForAccelerometerBlockGenerator::addInitAndTerminateCode(NxtOSEKRobotGenerator *nxtGen
			, QString const &port, qReal::Id const elementId)
{
	QString const initCode = "ecrobot_init_accel_sensor(" + port + ");";
	if (!ListSmartLine::isContains(nxtGen->initCode(), initCode)) {
		QString const terminateCode = "ecrobot_term_accel_sensorr(" + port + ");";
		nxtGen->initCode().append(SmartLine(initCode, elementId));
		nxtGen->terminateCode().append(SmartLine(terminateCode, elementId));
	}
}

QList<SmartLine> WaitForAccelerometerBlockGenerator::convertElementIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
		, qReal::Id const elementId, qReal::Id const logicElementId)
{
	QList<SmartLine> result;

	QString const port = "NXT_PORT_S" + nxtGen->api()->stringProperty(logicElementId, "Port");

	result.append(SmartLine("ecrobot_get_accel_sensor(" + port
							+ ") " + ", "
							"buf[3]" + ");",elementId));

	QString const acceleration = nxtGen->api()->stringProperty(logicElementId, "Acceleration");
	QString const inequalitySign = transformSign(QString(nxtGen->api()->stringProperty(logicElementId
			, "Sign").toUtf8()));

	QString const condition = inequalitySign + " " + acceleration;

	result.append(SmartLine("while (!(ecrobot_get_accel_sensor(" + port
			+ ") " + condition + "))", elementId));
	result.append(SmartLine("{", elementId));
	result.append(SmartLine("	ecrobot_get_accel_sensor(" + port
							+ ") " + ", "
							"buf[3]" + ");",elementId));
	result.append(SmartLine("}", elementId));

	addInitAndTerminateCode(nxtGen, port, elementId);
	return result;
}
