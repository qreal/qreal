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

	int const enterButtonClicks = nxtGen->api()->stringProperty(logicElementId, "CentralButtonClicks").toInt();
	int const runButtonClicks = nxtGen->api()->stringProperty(logicElementId, "BottomButtonClicks").toInt();

	result.append(SmartLine("int runCounter = 0;", elementId));
	result.append(SmartLine("int enterCounter = 0;", elementId));
	result.append(SmartLine("bool enterWasDown = false;", elementId));
	result.append(SmartLine("bool runWasDown = false;", elementId));
	result.append(SmartLine("while (runCounter < " + QString::number(runButtonClicks)
							+ " || enterCounter < " + QString::number(enterButtonClicks) + ") {", elementId));
	result.append(SmartLine("if (!ecrobot_is_ENTER_button_pressed() && enterWasDown) {enterCounter++; enterWasDown = false;}", elementId));
	result.append(SmartLine("if (ecrobot_is_ENTER_button_pressed()) {enterWasDown = true;}", elementId));
	result.append(SmartLine("if (!ecrobot_is_RUN_button_pressed() && runWasDown) {runCounter++; runWasDown = false;}", elementId));
	result.append(SmartLine("if (ecrobot_is_RUN_button_pressed()) {runWasDown = true;}", elementId));
	result.append(SmartLine("}", elementId));
	return result;
}
