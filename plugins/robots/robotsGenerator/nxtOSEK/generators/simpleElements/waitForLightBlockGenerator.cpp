#include "waitForLightBlockGenerator.h"
#include "../../nxtOSEKRobotGenerator.h"

using namespace robots::generator;

WaitForLightBlockGenerator::WaitForLightBlockGenerator()
{
}

QList<SmartLine> WaitForLightBlockGenerator::convertElememtIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
		, qReal::Id const elementId, qReal::Id const logicElementId)
{
	QList<SmartLine> result;

	int const port = nxtGen->api()->stringProperty(logicElementId, "Port").toInt();

	QString const percents = nxtGen->api()->stringProperty(logicElementId,  "Percents");
	QString const inequalitySign = transformSign(QString(nxtGen->api()->stringProperty(logicElementId
			, "Sign").toUtf8()));

	QString const condition = inequalitySign + " " + percents;

	result.append(SmartLine("while (!(ecrobot_get_light_sensor(NXT_PORT_S" + QString::number(port)
			+ ") " + condition + "))", elementId));
	result.append(SmartLine("{", elementId));
	result.append(SmartLine("}", elementId));

	return result;
}
