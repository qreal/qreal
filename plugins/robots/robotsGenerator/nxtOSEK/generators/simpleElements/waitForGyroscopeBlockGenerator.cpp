#include "waitForGyroscopeBlockGenerator.h"
#include "../../nxtOSEKRobotGenerator.h"
#include "../listsmartline.h"

using namespace robots::generator;

WaitForGyroscopeBlockGenerator::WaitForGyroscopeBlockGenerator()
{
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

	result.append(SmartLine("while (!(ecrobot_get_gyro_sensor(" + port
			+ ") " + condition + "))", elementId));
	result.append(SmartLine("{", elementId));
	result.append(SmartLine("}", elementId));

	return result;
}
