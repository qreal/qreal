/* Copyright 2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#define CRASHCODE -1 // если exec ничего не возвратить, а такое бывает?
#define FREEZECODE -2

#include "QRealGuiTests.h"
#include "startQreal.cpp"
#include "dialogs/projectManagement/suggestToCreateProjectDialog.h"
#include "dialogs/projectManagement/suggestToCreateDiagramWidget.h"
#include "qrgui/mainWindow/qrealApplication.h"
#include "qrgui/plugins/toolPluginInterface/usedInterfaces/mainWindowScriptAPIInterface.h"
#include "./qrgui/mainWindow/mainWindow.h"

#include <qrutils/widgetFinder.h>
#include <QtScript/QScriptContext>
#include <QtScript/QScriptEngine>
#include <QtScript/QScriptValue>
#include <QFile>
#include <QTextStream>
#include <QtCore/QCoreApplication>
#include <QEventLoop>
#include <QTimer>
#include <QThread>
#include <QWidget>
#include <QDebug>
#include <QToolButton>
#include <QListWidget>

using namespace guiTesting;
using namespace qReal;
using namespace gui::expected;

QScriptValue scriptAssert(QScriptContext *context, QScriptEngine *engine)
{
	Q_UNUSED(engine);

	if (context->argumentCount() != 1) {
		ADD_FAILURE() << "'assert' shall have exactly one argument";
		return {};
	}

	if (!context->argument(0).toBool()) {
		ADD_FAILURE() << "Fail at\n" // FAIL() должен быть, можно бросать исключение!!!
						 // TODO! надо проверить, что будет, если просто бросать исключения. как тесты пойдут
						 // надо регистрировать исключения в engine походу
				<< QStringList(context->backtrace().mid(1)).join("\n").toStdString();
		return {};
	}

	return {};
}

QScriptValue scriptAddFailure(QScriptContext *context, QScriptEngine *engine)
{
	Q_UNUSED(engine);

	if (context->argumentCount() != 1) {
		ADD_FAILURE() << "'failure' shall have exactly one argument";
		return {};
	}

	if (!(context->argument(0).isValid() && context->argument(0).isString())) {
		ADD_FAILURE() << "Assertion failure at\n"
				<< QStringList(context->backtrace().mid(1)).join("\n").toStdString()
				<< "with the message:\n" << context->argument(0).toString().toStdString();
		return {};
	}
	// TODO: надо аборт написать и закрыть все окна, выйти из приложения.
	return {};
}

QScriptValue scriptExpect(QScriptContext *context, QScriptEngine *engine)
{
	Q_UNUSED(engine);

	if (context->argumentCount() != 1) {
		ADD_FAILURE() << "'expect' shall have exactly one argument";
		return {};
	}

	if (!context->argument(0).toBool()) {
		ADD_FAILURE() << "Expecting failure at\n"
				<< QStringList(context->backtrace().mid(1)).join("\n").toStdString();
		return {};
	}

	return {};
}

void QRealGuiTests::SetUp()
{
	SettingsManager::setValue("scriptInterpretation", true);

	MainWindow *window = start();
	mWindow = window;
	mWindow->setAttribute(Qt::WA_DeleteOnClose);

	mainWindowScriptAPIInterface = dynamic_cast<MainWidnowScriptAPIInterface *>(window);
	if (mainWindowScriptAPIInterface == nullptr) {
		FAIL() << "MainWindow is not found";
	}

	mainWindowScriptAPIInterface->registerNewFunction(scriptAssert, "assert");
	mainWindowScriptAPIInterface->registerNewFunction(scriptAddFailure, "failure");
	mainWindowScriptAPIInterface->registerNewFunction(scriptExpect, "expect"); // может стоит использовать? Kappa

	registerFunctionsWithTimer();

	mReturnCode = CRASHCODE;
	QTimer::singleShot(28000, [=]() {
		mainWindowScriptAPIInterface->abortEvaluation(); // не работает
		QApplication::instance()->exit(FREEZECODE);
	});
}

void QRealGuiTests::TearDown() // возможно стоит смотреть информацию с логов и что-либо там делать.
{
	SettingsManager::setValue("scriptInterpretation", mScriptInterpretationDefaultValue);
	QLOG_INFO() << "------------------- APPLICATION FINISHED -------------------";
	if (mReturnCode) {
		FAIL() << "Failed coz returnCode of the last app = " << std::to_string(mReturnCode);
	}
}

QString readFile(const QString &fileName)
{
	QFile scriptFile(fileName);
	if (!scriptFile.open(QIODevice::ReadOnly)) {
		ADD_FAILURE() << "Cant open file for reading for gui test: " << fileName.toStdString();
	}
	QTextStream stream(&scriptFile);
	QString contents = stream.readAll();
	scriptFile.close();
	return contents;
}

void QRealGuiTests::run(const QString &relativeFileName)
{
	QString scriptDirName = QApplication::applicationFilePath() +
			"/../../../qrtest/unitTests/guiTests/testScripts/" + scriptFolderName + "/";
	QString fileName = QDir::cleanPath(scriptDirName) + "/" + relativeFileName;
	QString script = readFile(fileName);

	checkScriptSyntax(script, relativeFileName);

	QTimer::singleShot(8000, [=]() { // возможно, фейлы тут работают не так, как должны
		mainWindowScriptAPIInterface->evaluateScript(script);
		checkLastEvaluating(relativeFileName);
		if (QApplication::activePopupWidget()) {
			QApplication::activePopupWidget()->close();
		}
		QApplication::closeAllWindows();
	});
	mReturnCode = QApplication::exec();
}

void QRealGuiTests::includeCommonScript(const QString &relativeFileName)
{
	QString scriptDirName = QApplication::applicationFilePath() + "/../../../qrtest/unitTests/guiTests/testScripts/";
	QString fileName = QDir::cleanPath(scriptDirName) + "/" + relativeFileName;
	QString script = readFile(fileName);

	checkScriptSyntax(script, relativeFileName);
	mainWindowScriptAPIInterface->evaluateScript(script);
	checkLastEvaluating(relativeFileName);
}

void QRealGuiTests::checkScriptSyntax(const QString &script, const QString &errorMsg)
{
	QScriptSyntaxCheckResult checkResult = mainWindowScriptAPIInterface->checkSyntax(script);
	if (checkResult.state() != QScriptSyntaxCheckResult::Valid) {
		QApplication::quit();
		FAIL() << "Failed coz code is invalide\n" << checkResult.errorMessage().toStdString() << "\n" << errorMsg.toStdString();

	}
}

void QRealGuiTests::checkLastEvaluating(const QString &errorMsg)
{
	if (mainWindowScriptAPIInterface->hasUncaughtException()) {
		std::string backtrace = mainWindowScriptAPIInterface->uncaughtExceptionBacktrace().join('\n').toStdString();
		mainWindowScriptAPIInterface->clearExceptions();
		if (QApplication::activePopupWidget()) {
			QApplication::activePopupWidget()->close();
		}
		QApplication::closeAllWindows();
		FAIL() << "Failed coz uncaughtException of the last evaluating exists\n" << backtrace << "\n" << errorMsg.toStdString();

	}
}

void QRealGuiTests::registerFunctionsWithTimer()
{
	// может унифицировать документацию к методам, которые что-то делают? а именнго указывать, мышка или клава используется и по чему ищется что-то?
	mainWindowScriptAPIInterface->registerNewFunction(scriptCloseContextMenu
													  , "closeContextMenu");
	mainWindowScriptAPIInterface->registerNewFunction(scriptActivateContextMenuAction
													  , "activateContextMenuAction");
	mainWindowScriptAPIInterface->registerNewFunction(scriptClickButtonInExpectedDialog
													  , "clickButtonInExpectedDialog");
	mainWindowScriptAPIInterface->registerNewFunction(scriptChooseComboBoxItemInExpectedDialog
													  , "chooseComboBoxListElementInExpectedDialog");
	mainWindowScriptAPIInterface->registerNewFunction(scriptFillLineEditInExpectedDialog
													  , "fillLineEditInExpectedDialog");
	mainWindowScriptAPIInterface->registerNewFunction(scriptClickButtonInExpectedTopLevelDialog
													  , "clickButtonInExpectedTopLevelDialog");
	mainWindowScriptAPIInterface->registerNewFunction(scriptChooseExpectedDialogDiagram
													  , "chooseExpectedDialogDiagram");


	mainWindowScriptAPIInterface->registerNewFunction(closeExpectedDialog
													  , "closeExpectedDialog");
}

TEST_F(QRealGuiTests, createRootElementOnSceneAndQuit)
{
	includeCommonScript("common.js");
	run("createRootElementOnSceneAndQuit.js");
}
//TEST_F(QRealGuiTests, dockWidgetsExistence)
//{
//	includeCommonScript("common.js");
//	run("dockWidgetsExistence.js");
//}
/*
TEST_F(QRealGuiTests, toolbarsElementsExistence)
{
	runFromFile("toolbarsElementsExistence.js");
}

TEST_F(QRealGuiTests, script1)
{
	runFromFile("script1.js");
}
// TODO: необходимо сменить ассерты на эксепты в нужных местах
TEST_F(QRealGuiTests, sanityCheck)
{
	run("assert(true);");
	ASSERT_EQ(2, 1 + 1);
}

TEST_F(QRealGuiTests, editActionsExistence)
{
	runFromFile("editActionsExistence.js"); // мб qs?
}

TEST_F(QRealGuiTests, viewActionsExistence)
{
	runFromFile("viewActionsExistence.js");
}

//TEST_F(QRealGuiTests, findDialogElementsExistence)
//{
//	runFromFile("findDialogElementsExistence.js");
//}

TEST_F(QRealGuiTests, helpActionsExistence)
{
	runFromFile("helpActionsExistence.js");
}

//TEST_F(QRealGuiTests, mouseGesturesElementsExistence)
//{
//	runFromFile("mouseGesturesElementsExistence.js");
//}

//TEST_F(QRealGuiTests, preferenceDialogElementsExistence)
//{
//	runFromFile("preferenceDialogElementsExistence.js");
//}

TEST_F(QRealGuiTests, settingsActionsExistence)
{
	runFromFile("settingsActionsExistence.js");
}

//TEST_F(QRealGuiTests, tabSceneExistence)
//{
//	runFromFile("tabSceneExistence.js");
//}



TEST_F(QRealGuiTests, toolsActionsExistence)
{
	runFromFile("toolsActionsExistence.js");
}

TEST_F(QRealGuiTests, fileActionsExistence)
{
	runFromFile("fileActionsExistence.js");
}

//TEST_F(QRealGuiTests, fileActionsFunctioning)
//{
//	runFromFile("fileActionsFunctioning.js");
//}

//TEST_F(QRealGuiTests, findDialogElementsFunctioning)
//{
//	runFromFile("findDialogElementsFunctioning.js");
//}

//TEST_F(QRealGuiTests, helpActionsFunctioning)
//{
//	runFromFile("helpActionsFunctioning.js");
//}

//TEST_F(QRealGuiTests, mainPanelsElementsFunctioning)
//{
//	runFromFile("mainPanelsElementsFunctioning.js");
//}

//TEST_F(QRealGuiTests, mouseGesturesElementsFunctioning)
//{
//	runFromFile("mouseGesturesElementsFunctioning.js");
//}

//TEST_F(QRealGuiTests, preferenceDialogElementsFunctioning)
//{
//	runFromFile("preferenceDialogElementsFunctioning.js");
//}

//TEST_F(QRealGuiTests, settingsActionsFunctioning)
//{
//	runFromFile("settingsActionsFunctioning.js");
//}

//TEST_F(QRealGuiTests, tabSceneFunctioning)
//{
//	runFromFile("tabSceneFunctioning.js");
//}

//TEST_F(QRealGuiTests, toolbarsElementsFunctioning)
//{
//	runFromFile("toolbarsElementsFunctioning.js");
//}

//TEST_F(QRealGuiTests, toolsActionsFunctioning)
//{
//	runFromFile("toolsActionsFunctioning.js");
//}

//TEST_F(QRealGuiTests, viewActionsFunctioning)
//{
//	runFromFile("viewActionsFunctioning.js");
//}

//TEST_F(QRealGuiTests, editActionsFunctioning)
//{
//	runFromFile("editActionsFunctioning.js");
//}

//TEST_F(QRealGuiTests, hotKeysExistanceAndFunctioning)
//{
//	runFromFile("hotKeysExistanceAndFunctioning.js");
//}



//TEST_F(QRealGuiTests, script2)
//{
//	runFromFile("script2.js");
//}

//TEST_F(QRealGuiTests, script3)
//{
//	runFromFile("script3.js");
//}

//TEST_F(QRealGuiTests, script4)
//{
//	runFromFile("script4.js");
//}

//TEST_F(QRealGuiTests, script5)
//{
//	runFromFile("script5.js");
//}

//TEST_F(QRealGuiTests, script6)
//{
//	runFromFile("script6.js");
//}

//TEST_F(QRealGuiTests, script7)
//{
//	runFromFile("script7.js");
//}

//TEST_F(QRealGuiTests, script8)
//{
//	runFromFile("script8.js");
//}

//TEST_F(QRealGuiTests, script9)
//{
//	runFromFile("script9.js");
//}

//TEST_F(QRealGuiTests, script10)
//{
//	runFromFile("script10.js");
//}
*/
