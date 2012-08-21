#include "waitForLightBlockGenerator.h"
#include "../../nxtOSEKRobotGenerator.h"
#include "../listsmartline.h"

using namespace robots::generator;

WaitForLightBlockGenerator::WaitForLightBlockGenerator()
{
}

void WaitForLightBlockGenerator::addInitAndTerminateCode(NxtOSEKRobotGenerator *nxtGen
			, QString const &port, qReal::Id const elementId)
{
	QString const initCode = "ecrobot_set_light_sensor_active(" + port + ");";
	if (!ListSmartLine::isContains(nxtGen->initCode(), initCode)) {
		QString const terminateCode = "ecrobot_set_light_sensor_inactive(" + port + ");";
		nxtGen->initCode().append(SmartLine(initCode, elementId));
		nxtGen->terminateCode().append(SmartLine(terminateCode, elementId));
	}
}

QList<SmartLine> WaitForLightBlockGenerator::convertElementIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
		, qReal::Id const elementId, qReal::Id const logicElementId)
{
	QList<SmartLine> result;

	QString const port = "NXT_PORT_S" + nxtGen->api()->stringProperty(logicElementId, "Port");

	QString const percents = nxtGen->api()->stringProperty(logicElementId, "Percents");
	QString const inequalitySign = transformSign(QString(nxtGen->api()->stringProperty(logicElementId
			, "Sign").toUtf8()));

	QString const condition = inequalitySign + " " + percents;

	result.append(SmartLine("while (!(ecrobot_get_light_sensor(" + port
			+ ") " + condition + "))", elementId));
	result.append(SmartLine("{", elementId));
	result.append(SmartLine("}", elementId));

	addInitAndTerminateCode(nxtGen, port, elementId);
	return result;
}
