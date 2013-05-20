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
	QString condition = "";

	int const enterButtonClicks = nxtGen->api()->stringProperty(logicElementId, "CentralButtonClicks").toInt();
	int const runButtonClicks = nxtGen->api()->stringProperty(logicElementId, "BottomButtonClicks").toInt();


	if(runButtonClicks){
		result.append(SmartLine("runCounter = 0;", elementId));
		result.append(SmartLine("runWasDown = 0;", elementId));
		condition += "runCounter < " + QString::number(runButtonClicks);
		nxtGen->runButtonUsed();
	}
	if(enterButtonClicks){
		result.append(SmartLine("enterCounter = 0;", elementId));
		result.append(SmartLine("enterWasDown = 0;", elementId));
		if(runButtonClicks){
			condition += " || ";
		}
		condition += "enterCounter < " + QString::number(enterButtonClicks);
		nxtGen->enterButtonUsed();
	}

	if(runButtonClicks || enterButtonClicks){
		result.append(SmartLine("while (" + condition + ") {", elementId, SmartLine::increase));
		if(enterButtonClicks){
			result.append(SmartLine("if (!ecrobot_is_ENTER_button_pressed() && enterWasDown) {", elementId, SmartLine::increase));
			result.append(SmartLine("enterCounter++;", elementId));
			result.append(SmartLine("enterWasDown = 0;", elementId));
			result.append(SmartLine("}", elementId, SmartLine::decrease));
			result.append(SmartLine("if (ecrobot_is_ENTER_button_pressed()) {", elementId, SmartLine::increase));
			result.append(SmartLine("enterWasDown = 1;", elementId));
			result.append(SmartLine("}", elementId, SmartLine::decrease));
		}
		if(runButtonClicks){
			result.append(SmartLine("if (!ecrobot_is_RUN_button_pressed() && runWasDown) {", elementId, SmartLine::increase));
			result.append(SmartLine("runCounter++;", elementId));
			result.append(SmartLine("runWasDown = 0;", elementId));
			result.append(SmartLine("}", elementId, SmartLine::decrease));
			result.append(SmartLine("if (ecrobot_is_RUN_button_pressed()) {", elementId, SmartLine::increase));
			result.append(SmartLine("runWasDown = 1;", elementId));
			result.append(SmartLine("}", elementId, SmartLine::decrease));
		}
		result.append(SmartLine("}", elementId, SmartLine::decrease));
		return result;
	}
}
