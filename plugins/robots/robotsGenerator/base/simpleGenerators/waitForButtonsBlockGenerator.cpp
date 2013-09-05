#include "waitForButtonsBlockGenerator.h"

using namespace qReal::robots::generators::simple;

WaitForButtonsBlockGenerator::WaitForButtonsBlockGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "wait/buttons.t", QList<Binding *>(), parent)
{
}

//QList<SmartLine_old> WaitForButtonsBlockGenerator::convertElementIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
//		, qReal::Id const &elementId, qReal::Id const &logicElementId)
//{
//	QList<SmartLine_old> result;
//	QString condition = "";

//	QString const enterButtonClicks = nxtGen->intExpression(logicElementId, "CentralButtonClicks");
//	QString const cancelButtonClicks = nxtGen->intExpression(logicElementId, "BottomButtonClicks");
//	bool const listenEnter = !enterButtonClicks.isEmpty() && enterButtonClicks.trimmed() != "0";
//	bool const listenCancel = !cancelButtonClicks.isEmpty() && cancelButtonClicks.trimmed() != "0";

//	if (listenCancel) {
//		result.append(SmartLine_old("cancelCounter = 0;", elementId));
//		result.append(SmartLine_old("cancelWasDown = 0;", elementId));
//		condition += "cancelCounter < " + cancelButtonClicks;
//		nxtGen->variables().cancelButtonUsed();
//	}
//	if (listenEnter) {
//		result.append(SmartLine_old("enterCounter = 0;", elementId));
//		result.append(SmartLine_old("enterWasDown = 0;", elementId));
//		if (listenCancel) {
//			condition += " || ";
//		}
//		condition += "enterCounter < " + enterButtonClicks;
//		nxtGen->variables().enterButtonUsed();
//	}

//	if (listenEnter || listenCancel) {
//		result.append(SmartLine_old("while (" + condition + ") {", elementId, SmartLine_old::increase));
//		if (listenEnter) {
//			result.append(SmartLine_old("if (!ecrobot_is_ENTER_button_pressed() && enterWasDown) {", elementId, SmartLine_old::increase));
//			result.append(SmartLine_old("enterCounter++;", elementId));
//			result.append(SmartLine_old("enterWasDown = 0;", elementId));
//			result.append(SmartLine_old("}", elementId, SmartLine_old::decrease));
//			result.append(SmartLine_old("if (ecrobot_is_ENTER_button_pressed()) {", elementId, SmartLine_old::increase));
//			result.append(SmartLine_old("enterWasDown = 1;", elementId));
//			result.append(SmartLine_old("}", elementId, SmartLine_old::decrease));
//		}
//		if (listenCancel) {
//			result.append(SmartLine_old("if (!ecrobot_is_RUN_button_pressed() && cancelWasDown) {", elementId, SmartLine_old::increase));
//			result.append(SmartLine_old("cancelCounter++;", elementId));
//			result.append(SmartLine_old("cancelWasDown = 0;", elementId));
//			result.append(SmartLine_old("}", elementId, SmartLine_old::decrease));
//			result.append(SmartLine_old("if (ecrobot_is_RUN_button_pressed()) {", elementId, SmartLine_old::increase));
//			result.append(SmartLine_old("cancelWasDown = 1;", elementId));
//			result.append(SmartLine_old("}", elementId, SmartLine_old::decrease));
//		}
//		result.append(SmartLine_old("}", elementId, SmartLine_old::decrease));
//		return result;
//	}
//	return result;
//}
