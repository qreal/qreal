#include "waitForButtonsBlockGenerator.h"
#include "../../nxtOSEKRobotGenerator.h"
#include "../listsmartline.h"

using namespace robots::generator;

WaitForButtonsBlockGenerator::WaitForButtonsBlockGenerator()
{
}

QList<SmartLine> WaitForButtonsBlockGenerator::convertElementIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
		, qReal::Id const &elementId, qReal::Id const &logicElementId)
{
	QList<SmartLine> result;
	QString condition = "";

	QString const enterButtonClicks = nxtGen->intExpression(logicElementId, "CentralButtonClicks");
	QString const cancelButtonClicks = nxtGen->intExpression(logicElementId, "BottomButtonClicks");
	bool const listenEnter = !enterButtonClicks.isEmpty() && enterButtonClicks.trimmed() != "0";
	bool const listenCancel = !cancelButtonClicks.isEmpty() && cancelButtonClicks.trimmed() != "0";

	if (listenCancel) {
		result.append(SmartLine("cancelCounter = 0;", elementId));
		result.append(SmartLine("cancelWasDown = 0;", elementId));
		condition += "cancelCounter < " + cancelButtonClicks;
		nxtGen->variables().cancelButtonUsed();
	}
	if (listenEnter) {
		result.append(SmartLine("enterCounter = 0;", elementId));
		result.append(SmartLine("enterWasDown = 0;", elementId));
		if (listenCancel) {
			condition += " || ";
		}
		condition += "enterCounter < " + enterButtonClicks;
		nxtGen->variables().enterButtonUsed();
	}

	if (listenEnter || listenCancel) {
		result.append(SmartLine("while (" + condition + ") {", elementId, SmartLine::increase));
		if (listenEnter) {
			result.append(SmartLine("if (!ecrobot_is_ENTER_button_pressed() && enterWasDown) {", elementId, SmartLine::increase));
			result.append(SmartLine("enterCounter++;", elementId));
			result.append(SmartLine("enterWasDown = 0;", elementId));
			result.append(SmartLine("}", elementId, SmartLine::decrease));
			result.append(SmartLine("if (ecrobot_is_ENTER_button_pressed()) {", elementId, SmartLine::increase));
			result.append(SmartLine("enterWasDown = 1;", elementId));
			result.append(SmartLine("}", elementId, SmartLine::decrease));
		}
		if (listenCancel) {
			result.append(SmartLine("if (!ecrobot_is_RUN_button_pressed() && cancelWasDown) {", elementId, SmartLine::increase));
			result.append(SmartLine("cancelCounter++;", elementId));
			result.append(SmartLine("cancelWasDown = 0;", elementId));
			result.append(SmartLine("}", elementId, SmartLine::decrease));
			result.append(SmartLine("if (ecrobot_is_RUN_button_pressed()) {", elementId, SmartLine::increase));
			result.append(SmartLine("cancelWasDown = 1;", elementId));
			result.append(SmartLine("}", elementId, SmartLine::decrease));
		}
		result.append(SmartLine("}", elementId, SmartLine::decrease));
		return result;
	}
	return result;
}
