#include "waitForTouchSensorBlockGenerator.h"
#include "../../nxtOSEKRobotGenerator.h"

using namespace robots::generator;

WaitForTouchSensorBlockGenerator::WaitForTouchSensorBlockGenerator()
{
}

QList<SmartLine> WaitForTouchSensorBlockGenerator::convertElememtIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
		, qReal::Id const elementId, qReal::Id const logicElementId)
{
	QList<SmartLine> result;

	int const port = nxtGen->api()->stringProperty(logicElementId, "Port").toInt();

	result.append(SmartLine("while (!ecrobot_get_touch_sensor(NXT_PORT_S" + QString::number(port) + "))"
			, elementId));
	result.append(SmartLine("{", elementId));
	result.append(SmartLine("}", elementId));

	return result;
}
