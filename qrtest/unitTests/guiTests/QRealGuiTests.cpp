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

static const int CRASHCODE = -1;
static const int FREEZECODE = -2;

#include "QRealGuiTests.h"

#include "startQreal.cpp"
#include "dialogs/projectManagement/suggestToCreateProjectDialog.h"
#include "dialogs/projectManagement/suggestToCreateDiagramWidget.h"
#include "qrgui/mainWindow/qrealApplication.h"
#include "workaroundTestFunctions.h"
#include "TestAgent.h"
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
#include <QTest>

using namespace qReal;
using namespace guiTesting;
using namespace guiTesting::workarounds;
using namespace qReal::gui;

QScriptValue scriptAssert(QScriptContext *context, QScriptEngine *engine)
{
	Q_UNUSED(engine);

	if (context->argumentCount() != 1) {
		ADD_FAILURE() << "'assert' shall have exactly one argument. Fail at\n"
				<< QStringList(context->backtrace().mid(1)).join("\n").toStdString();
		context->throwError("incorrect assert failure: more than one arg");
		return {};
	}

	if (!context->argument(0).toBool()) {
		ADD_FAILURE() << "Fail at\n"
				<< QStringList(context->backtrace().mid(1)).join("\n").toStdString();
		context->throwError("assert failure");
		return {};
	}
	return {};
}

QScriptValue scriptFail(QScriptContext *context, QScriptEngine *engine)
{
	Q_UNUSED(engine);

	if (context->argumentCount() > 1) {
		ADD_FAILURE() << "'fail' shall have exactly 0 or 1 argument";
		context->throwError("failed when fail() was invoked: more than one arg");
		return {};
	}
	if (context->argumentCount() == 1) {
		if (!(context->argument(0).isValid() && context->argument(0).isString())) {
			ADD_FAILURE() << "incorrect fail() at\n"
					<< QStringList(context->backtrace().mid(1)).join("\n").toStdString();
			context->throwError("incorrect fail()");
			return {};
		}
	}

	QString errorMsg = (context->argumentCount() == 1) ? context->argument(0).toString() : "";
	ADD_FAILURE() << "fail() at\n"
			<< QStringList(context->backtrace().mid(1)).join("\n").toStdString()
			<< "with a msg:\n" << errorMsg.toStdString();
	context->throwError("fail() with msg: " + errorMsg);
	return {};
}

QScriptValue scriptAddFailure(QScriptContext *context, QScriptEngine *engine)
{
	Q_UNUSED(engine);

	if (context->argumentCount() > 1) {
		ADD_FAILURE() << "'add_failure()' shall have exactly 0 or 1 argument";
		return {};
	}
	if (context->argumentCount() == 1) {
		if (!(context->argument(0).isValid() && context->argument(0).isString())) {
			ADD_FAILURE() << "incorrect add_failure() at\n"
					<< QStringList(context->backtrace().mid(1)).join("\n").toStdString();
			return {};
		}
	}

	QString errorMsg = (context->argumentCount() == 1) ? context->argument(0).toString() : "";
	ADD_FAILURE() << "add_failure() at\n"
			<< QStringList(context->backtrace().mid(1)).join("\n").toStdString()
			<< "with a msg:\n" << errorMsg.toStdString();
	return {};
}

QScriptValue scriptExpect(QScriptContext *context, QScriptEngine *engine)
{
	Q_UNUSED(engine);

	if (context->argumentCount() != 1) {
		ADD_FAILURE() << "'expect()' shall have exactly one argument. Fail at\n"
				<< QStringList(context->backtrace().mid(1)).join("\n").toStdString();
		return {};
	}

	if (!context->argument(0).toBool()) {
		ADD_FAILURE() << "Unexpected value at\n"
				<< QStringList(context->backtrace().mid(1)).join("\n").toStdString();
		return {};
	}
	return {};
}

void QRealGuiTests::SetUp()
{
	MainWindow *window = start();
	mWindow = window;
	mWindow->setAttribute(Qt::WA_DeleteOnClose);

	mMainWindowScriptAPIInterface = dynamic_cast<MainWidnowScriptAPIInterface *>(window);
	if (mMainWindowScriptAPIInterface == nullptr) {
		FAIL() << "MainWindow is not found";
	}

	mMainWindowScriptAPIInterface->registerNewFunction(scriptAssert, "assert");
	mMainWindowScriptAPIInterface->registerNewFunction(scriptFail, "fail");
	mMainWindowScriptAPIInterface->registerNewFunction(scriptAddFailure, "add_failure");
	mMainWindowScriptAPIInterface->registerNewFunction(scriptExpect, "expect");

	// ATTENTION: workarounds
	mMainWindowScriptAPIInterface->registerNewFunction(reachedEndOfScript, "reachedEndOfScript");
	mMainWindowScriptAPIInterface->registerNewFunction(closeExpectedDialog, "closeExpectedDialog");
	mMainWindowScriptAPIInterface->registerNewFunction(chooseExpectedDialogDiagram, "chooseExpectedDialogDiagram");

	QScriptEngine *engine = mMainWindowScriptAPIInterface->getEngine();
	mTestAgent = new TestAgent(engine);
	// engine->setAgent(mTestAgent); // for writting and debugging. dont remove!

	mReturnCode = CRASHCODE;
	QTimer *timer = new QTimer(window);
	timer->setSingleShot(true);
	QObject::connect(timer, &QTimer::timeout, [this]() { failTest(); });
	timer->start(mTimeLimit);
}

// It may be usefull to use the LOG for some information about a failed/passed/running test
void QRealGuiTests::TearDown()
{
	mMainWindowScriptAPIInterface = nullptr;
	mCurrentIncludedFiles.clear();
	mCurrentTotalProgram.clear();
	mCurrentEvaluatingScript.clear();
	mCurrentEvaluatingScriptFile.clear();
	mScript.clear();
	mFileName.clear();
	mRelativeName.clear();

	QLOG_INFO() << "------------------- APPLICATION FINISHED -------------------";
	QLOG_INFO() << "with exit code " << QString::number(mReturnCode);

	if (mReturnCode) {
		ADD_FAILURE() << "ERROR: returnCode of the last app = " << std::to_string(mReturnCode);
	}
	if (!SettingsManager::value("reachedEndOfFile").toBool()) {
		FAIL() << "Failed coz end of the script has not been reached\n";
	}
}

void QRealGuiTests::run(const QString &relativeFileName)
{
	QString scriptDirName = QApplication::applicationFilePath() +
			"/../../../qrtest/unitTests/guiTests/testScripts/" + mScriptFolderName + "/";
	QString fileName = QDir::cleanPath(scriptDirName) + "/" + relativeFileName;
	QString script = readFile(fileName);
	mCurrentEvaluatingScriptFile = relativeFileName;
	mCurrentEvaluatingScript = script;
	mCurrentTotalProgram += script;
	prepareScriptForRunning(script);
	if (::testing::Test::HasFailure()) {
		return;
	}
	checkScriptSyntax(script, relativeFileName);
	if (::testing::Test::HasFailure()) {
		return;
	}

	mScript = script;
	mFileName = fileName;
	mRelativeName = relativeFileName;

	QTimer *timer = new QTimer(mWindow);
	timer->setSingleShot(true);
	QObject::connect(timer, &QTimer::timeout, [this]() {
		if (::testing::Test::HasFailure()) {
			return;
		}
		mMainWindowScriptAPIInterface->evaluateScript(mScript, mFileName);
		checkLastEvaluating(mRelativeName);
		if (::testing::Test::HasFailure()) {
			return;
		}
		if (QApplication::activePopupWidget()) {
			QApplication::activePopupWidget()->close();
		}
		QApplication::closeAllWindows();
	});
	timer->start(mTimeToExpose);
	mReturnCode = QApplication::exec();
}

void QRealGuiTests::includeCommonScript(const QString &relativeFileName)
{
	QString scriptDirName = QApplication::applicationFilePath() +
			"/../../../qrtest/unitTests/guiTests/testScripts/";
	QString fileName = QDir::cleanPath(scriptDirName) + "/" + relativeFileName;
	QString script = readFile(fileName);
	mCurrentEvaluatingScriptFile = relativeFileName;
	mCurrentIncludedFiles << fileName;
	mCurrentEvaluatingScript = script;
	mCurrentTotalProgram += script;
	prepareScriptForRunning(script);
	if (::testing::Test::HasFailure()) {
		return;
	}
	checkScriptSyntax(script, relativeFileName);
	if (::testing::Test::HasFailure()) {
		return;
	}
	mMainWindowScriptAPIInterface->evaluateScript(script, fileName);
	checkLastEvaluating(relativeFileName);
}

void QRealGuiTests::failTest()
{
	mMainWindowScriptAPIInterface->abortEvaluation();
	mMainWindowScriptAPIInterface->getEngine()->currentContext()->throwError("failTest: freeze");
	exterminate(FREEZECODE);
}

QString QRealGuiTests::getScriptFolderName() const
{
	return mScriptFolderName;
}

void QRealGuiTests::setScriptFolderName(const QString &folder)
{
	mScriptFolderName = folder;
}

int QRealGuiTests::getTimeToExpose() const
{
	return mTimeToExpose;
}

void QRealGuiTests::setTimeToExpose(const int timeToExpose)
{
	mTimeToExpose = timeToExpose;
}

int QRealGuiTests::getTimeLimit() const
{
	return mTimeLimit;
}

void QRealGuiTests::setTimeLimit(const int timeLimit)
{
	mTimeLimit = timeLimit;
}

void QRealGuiTests::checkScriptSyntax(const QString &script, const QString &errorMsg)
{
	QScriptSyntaxCheckResult checkResult = mMainWindowScriptAPIInterface->checkSyntax(script);
	if (checkResult.state() != QScriptSyntaxCheckResult::Valid) {
		QApplication::quit();
		FAIL() << "Failed coz code is invalide\n" << checkResult.errorMessage().toStdString()
			   << "\n" << errorMsg.toStdString();
	}
}

void QRealGuiTests::checkLastEvaluating(const QString &errorMsg)
{
	if (mMainWindowScriptAPIInterface->hasUncaughtException()) {
		std::string backtrace = mMainWindowScriptAPIInterface->uncaughtExceptionBacktrace().join('\n').toStdString();
		mMainWindowScriptAPIInterface->clearExceptions();
		if (QApplication::activePopupWidget()) {
			QApplication::activePopupWidget()->close();
		}
		QApplication::closeAllWindows();
		FAIL() << "Failed coz uncaughtException of the last evaluating exists\n" << backtrace << "\n" << errorMsg.toStdString();
	}
}

QString QRealGuiTests::readFile(const QString &fileName)
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

void QRealGuiTests::prepareScriptForRunning(QString &script)
{
	script += "reachedEndOfScript();\n";
	SettingsManager::setValue("reachedEndOfFile", "false");
}

void QRealGuiTests::exterminate(const int returnCode)
{
	while (QApplication::activePopupWidget()) {
		QApplication::activePopupWidget()->close();
	}
	while (QApplication::activeModalWidget()) {
		QApplication::activeModalWidget()->close();
	}
	QApplication::instance()->exit(returnCode);
}

TEST_F(QRealGuiTests, dockWidgetsExistence)
{
	includeCommonScript("common.js");
	run("dockWidgetsExistence.js");
}

TEST_F(QRealGuiTests, createRootElementOnSceneAndQuit)
{
	includeCommonScript("common.js");
	run("createRootElementOnSceneAndQuit.js");
}

TEST_F(QRealGuiTests, fileActionsExistence)
{
	includeCommonScript("common.js");
	run("fileActionsExistence.js");
}

/*
// Existence
TEST_F(QRealGuiTests, dockWidgetsExistence)
{
	includeCommonScript("common.js");
	run("dockWidgetsExistence.js");
}

TEST_F(QRealGuiTests, editActionsExistence)
{
	includeCommonScript("common.js");
	run("editActionsExistence.js");
}

TEST_F(QRealGuiTests, fileActionsExistence)
{
	includeCommonScript("common.js");
	run("fileActionsExistence.js");
}

TEST_F(QRealGuiTests, findDialogElementsExistence)
{
	includeCommonScript("common.js");
	run("findDialogElementsExistence.js");
}

TEST_F(QRealGuiTests, helpActionsExistence)
{
	includeCommonScript("common.js");
	run("helpActionsExistence.js");
}

TEST_F(QRealGuiTests, mouseGesturesElementsExistence)
{
	includeCommonScript("common.js");
	run("mouseGesturesElementsExistence.js");
}

TEST_F(QRealGuiTests, preferenceDialogElementsExistence)
{
	includeCommonScript("common.js");
	run("preferenceDialogElementsExistence.js");
}

TEST_F(QRealGuiTests, settingsActionsExistence)
{
	includeCommonScript("common.js");
	run("settingsActionsExistence.js");
}

TEST_F(QRealGuiTests, toolbarsElementsExistence)
{
	includeCommonScript("common.js");
	run("toolbarsElementsExistence.js");
}

TEST_F(QRealGuiTests, toolsActionsExistence)
{
	includeCommonScript("common.js");
	run("toolsActionsExistence.js");
}

TEST_F(QRealGuiTests, viewActionsExistence)
{
	includeCommonScript("common.js");
	run("viewActionsExistence.js");
}

// Functioning
TEST_F(QRealGuiTests, dockWidgetsFunctioning)
{
	includeCommonScript("common.js");
	run("dockWidgetsFunctioning.js");
}

TEST_F(QRealGuiTests, editActionsFunctioning)
{
	includeCommonScript("common.js");
	run("editActionsFunctioning.js");
}

TEST_F(QRealGuiTests, fileActionsFunctioning)
{
	includeCommonScript("common.js");
	run("fileActionsFunctioning.js");
}

TEST_F(QRealGuiTests, findDialogElementsFunctioning)
{
	includeCommonScript("common.js");
	run("findDialogElementsFunctioning.js");
}

TEST_F(QRealGuiTests, helpActionsFunctioning)
{
	includeCommonScript("common.js");
	run("helpActionsFunctioning.js");
}

TEST_F(QRealGuiTests, preferenceDialogElementsFunctioning)
{
	includeCommonScript("common.js");
	run("preferenceDialogElementsFunctioning.js");
}

TEST_F(QRealGuiTests, toolbarsElementsFunctioning)
{
	includeCommonScript("common.js");
	run("toolbarsElementsFunctioning.js");
}

TEST_F(QRealGuiTests, toolsActionsFunctioning)
{
	includeCommonScript("common.js");
	run("toolsActionsFunctioning.js");
}

TEST_F(QRealGuiTests, viewActionsFunctioning)
{
	includeCommonScript("common.js");
	run("viewActionsFunctioning.js");
}

// Different complicated scripts
TEST_F(QRealGuiTests, autoSaveWork)
{
	includeCommonScript("common.js");
	run("autoSaveWork.js");
}

TEST_F(QRealGuiTests, createDiagramInCurrentModel)
{
	includeCommonScript("common.js");
	run("createDiagramInCurrentModel.js");
}

TEST_F(QRealGuiTests, createRootElementOnSceneAndQuit)
{
	includeCommonScript("common.js");
	run("createRootElementOnSceneAndQuit.js");
}

TEST_F(QRealGuiTests, fullscreenModeScript)
{
	includeCommonScript("common.js");
	run("fullscreenModeScript.js");
}

TEST_F(QRealGuiTests, linkNodeCooperationScript)
{
	includeCommonScript("common.js");
	run("linkNodeCooperationScript.js");
}

TEST_F(QRealGuiTests, logicalGraphicalModelsAndSceneInteraction)
{
	includeCommonScript("common.js");
	run("logicalGraphicalModelsAndSceneInteraction.js");
}

TEST_F(QRealGuiTests, miniMapWorkScript)
{
	includeCommonScript("common.js");
	run("miniMapWorkScript.js");
}

TEST_F(QRealGuiTests, paletteAndEditorPropertiesConcordance)
{
	includeCommonScript("common.js");
	run("paletteAndEditorPropertiesConcordance.js");
}

TEST_F(QRealGuiTests, paletteSearchScript)
{
	includeCommonScript("common.js");
	run("paletteSearchScript.js");
}

TEST_F(QRealGuiTests, propertyEditorScript)
{
	includeCommonScript("common.js");
	run("propertyEditorScript.js");
}

TEST_F(QRealGuiTests, rightclickAllScreenScript)
{
	includeCommonScript("common.js");
	run("rightclickAllScreenScript.js");
}

TEST_F(QRealGuiTests, saveOpenScript)
{
	includeCommonScript("common.js");
	run("saveOpenScript.js");
}

TEST_F(QRealGuiTests, verySimpleShapeEditorTest)
{
	includeCommonScript("common.js");
	run("verySimpleShapeEditorTest.js");
}
*/
