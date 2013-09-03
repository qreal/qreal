#include "waitForSonarBlockGenerator.h"
#include "../../nxtOSEKRobotGenerator.h"
#include "../listsmartline.h"

using namespace robots::generator;

// TODO: make it customizable
int const sleepInterval = 10;

WaitForSonarBlockGenerator::WaitForSonarBlockGenerator()
{
}

void WaitForSonarBlockGenerator::addInitAndTerminateCode(NxtOSEKRobotGenerator *nxtGen
			, QString const &port, qReal::Id const elementId)
{
	QString const initCode = "ecrobot_init_sonar_sensor(" + port + ");";
	if (!ListSmartLine::isContains(nxtGen->initCode(), initCode)) {
		QString const terminateCode = "ecrobot_term_sonar_sensor(" + port + ");";
		nxtGen->initCode().append(SmartLine(initCode, elementId));
		nxtGen->terminateCode().append(SmartLine(terminateCode, elementId));
	}
}
QList<SmartLine> WaitForSonarBlockGenerator::convertElementIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
		, qReal::Id const elementId, qReal::Id const logicElementId)
{
	QList<SmartLine> result;

	QString const port = "NXT_PORT_S" + nxtGen->api()->stringProperty(logicElementId, "Port");
	QString const distance = nxtGen->api()->stringProperty(logicElementId, "Distance");
	QString const inequalitySign = transformSign(QString(nxtGen->api()->stringProperty(logicElementId
			, "Sign").toUtf8()));
	QString const condition = inequalitySign + " " + distance;

	result.append(SmartLine("while (!(ecrobot_get_sonar_sensor(" + port + ") "
			+ condition + ")) {", elementId, SmartLine::increase));
	result.append(SmartLine(QString("systick_wait_ms(%1);").arg(
			QString::number(sleepInterval)), elementId));
	result.append(SmartLine("}", elementId, SmartLine::decrease));

	addInitAndTerminateCode(nxtGen, port, elementId);

	return result;
}
