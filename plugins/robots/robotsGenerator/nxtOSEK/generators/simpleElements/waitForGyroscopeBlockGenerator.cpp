#include "waitForGyroscopeBlockGenerator.h"
#include "../../nxtOSEKRobotGenerator.h"
#include "../listsmartline.h"

using namespace robots::generator;

WaitForGyroscopeBlockGenerator::WaitForGyroscopeBlockGenerator()
{
}

void WaitForGyroscopeBlockGenerator::addInitAndTerminateCode(NxtOSEKRobotGenerator *nxtGen
			, QString const &port, qReal::Id const elementId)
{
	QString const initCode = "ecrobot_set_gyroscope_sensor_active(" + port + ");";
	if (!ListSmartLine::isContains(nxtGen->initCode(), initCode)) {
		QString const terminateCode = "ecrobot_set_gyroscope_sensor_inactive(" + port + ");";
		nxtGen->initCode().append(SmartLine(initCode, elementId));
		nxtGen->terminateCode().append(SmartLine(terminateCode, elementId));
	}
}

QList<SmartLine> WaitForGyroscopeBlockGenerator::convertElementIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
		, qReal::Id const elementId, qReal::Id const logicElementId)
{
	QList<SmartLine> result;

	QString const port = "NXT_PORT_S" + nxtGen->api()->stringProperty(logicElementId, "Port");

	QString const degrees = nxtGen->api()->stringProperty(logicElementId, "Degrees");
	QString const inequalitySign = transformSign(QString(nxtGen->api()->stringProperty(logicElementId
			, "Sign").toUtf8()));

	QString const condition = inequalitySign + " " + degrees;

	result.append(SmartLine("while (!(ecrobot_get_gyroscope_sensor(" + port
			+ ") " + condition + "))", elementId));
	result.append(SmartLine("{", elementId));
	result.append(SmartLine("}", elementId));

	addInitAndTerminateCode(nxtGen, port, elementId);
	return result;
}
