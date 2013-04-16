#include "waitForButtonsBlockGenerator.h"
#include "../../nxtOSEKRobotGenerator.h"
#include "../listsmartline.h"

using namespace robots::generator;

WaitForButtonsBlockGenerator::WaitForButtonsBlockGenerator()
{
}

QList<SmartLine> WaitForButtonsBlockGenerator::convertElementIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
		, qReal::Id const elementId, qReal::Id const logicElementId)
{
	QList<SmartLine> result;

	//int const enterButtonClicks = nxtGen->api()->stringProperty(logicElementId, "Port").toInt();

	//result.append(SmartLine("while (!ecrobot_get_touch_sensor(NXT_PORT_S" + QString::number(port) + "))"
	//		, elementId));
	result.append(SmartLine("{", elementId));
	result.append(SmartLine("}", elementId));

	return result;
}
