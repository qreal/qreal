#include "waitForSonarBlockGenerator.h"
#include "../../nxtOSEKRobotGenerator.h"

using namespace robots::generator;

WaitForSonarBlockGenerator::WaitForSonarBlockGenerator()
{
}

QList<SmartLine> WaitForSonarBlockGenerator::convertElememtIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
		, qReal::Id const elementId, qReal::Id const logicElementId)
{
	QList<SmartLine> result;

	int const port = nxtGen->api()->stringProperty(logicElementId, "Port").toInt();
	QString const distance = nxtGen->api()->stringProperty(logicElementId, "Distance");
	QString const inequalitySign = transformSign(QString(nxtGen->api()->stringProperty(logicElementId
			, "Sign").toUtf8()));
	QString const condition = inequalitySign + " " + distance;

	result.append(SmartLine("while (!(ecrobot_get_sonar_sensor(NXT_PORT_S" + QString::number(port) + ") "
			+ condition + "))", elementId));
	result.append(SmartLine("{", elementId));
	result.append(SmartLine("}", elementId));

	return result;
}
