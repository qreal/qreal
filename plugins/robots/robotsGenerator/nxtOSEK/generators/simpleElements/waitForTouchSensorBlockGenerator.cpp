#include "waitForTouchSensorBlockGenerator.h"
#include "../../nxtOSEKRobotGenerator.h"

using namespace robots::generator;

WaitForTouchSensorBlockGenerator::WaitForTouchSensorBlockGenerator()
{
}

QList<SmartLine_old> WaitForTouchSensorBlockGenerator::convertElementIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
		, qReal::Id const &elementId, qReal::Id const &logicElementId)
{
	QList<SmartLine_old> result;

	int const port = nxtGen->api()->stringProperty(logicElementId, "Port").toInt();

	result.append(SmartLine_old("while (!ecrobot_get_touch_sensor(NXT_PORT_S" + QString::number(port) + ")) {"
			, elementId));
	result.append(SmartLine_old("}", elementId));

	return result;
}
