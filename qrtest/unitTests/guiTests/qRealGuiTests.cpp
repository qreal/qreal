/* Copyright 2015-2016 QReal Research Group, CyberTech Labs Ltd.
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

#include "qRealGuiTests.h"

#include "startQReal.cpp"
#include "workaroundTestFunctions.h"
#include "testAgent.h"
#include <dialogs/projectManagement/suggestToCreateProjectDialog.h>
#include <dialogs/projectManagement/suggestToCreateDiagramWidget.h>
#include <qrgui/mainWindow/qrealApplication.h>
#include <mainWindow/scriptingControlAPI.h>
#include <qrutils/widgetFinder.h>
#include <qrkernel/definitions.h>

#include <QtScript/QScriptContext>
#include <QtScript/QScriptEngine>
#include <QtScript/QScriptValue>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QCoreApplication>
#include <QtCore/QEventLoop>
#include <QtCore/QTimer>
#include <QtCore/QThread>
#include <QtWidgets/QWidget>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QListWidget>
#include <QtTest/QTest>

using namespace qReal;
using namespace guiTesting;
using namespace guiTesting::workarounds;
using namespace qReal::gui;

QScriptValue scriptAssert(QScriptContext *context, QScriptEngine *engine)
{
	Q_UNUSED(engine);
	const QString backtrace = QStringList(context->backtrace().mid(1)).join("\n");
	if (context->argumentCount() != 1) {
		ADD_FAILURE() << "'assert(...)' must have exactly one argument. Fail at\n" << qPrintable(backtrace);
		context->throwError(QObject::tr("Incorrect assertion expression: should have exactly 1"
				" argument at %1").arg(backtrace));
		return {};
	}

	if (!context->argument(0).toBool()) {
		ADD_FAILURE() << "Fail at\n" << qPrintable(backtrace);
		context->throwError(QObject::tr("Assert failure at %1").arg(backtrace));
		return {};
	}

	return {};
}

QScriptValue scriptFail(QScriptContext *context, QScriptEngine *engine)
{
	Q_UNUSED(engine);
	const QString backtrace = QStringList(context->backtrace().mid(1)).join("\n");
	if (context->argumentCount() > 1) {
		ADD_FAILURE() << "'fail(...)' must have exactly 0 or 1 argument";
		context->throwError(QObject::tr("Failed when fail() was invoked: more than one argument, at %1")
				.arg(backtrace));
		return {};
	}

	if (context->argumentCount() == 1) {
		if (!(context->argument(0).isValid() && context->argument(0).isString())) {
			ADD_FAILURE() << "incorrect fail() at\n" << qPrintable(backtrace);
			context->throwError(QObject::tr("Incorrect fail() called at %1").arg(backtrace));
			return {};
		}
	}

	QString errorMsg = (context->argumentCount() == 1) ? context->argument(0).toString() : "";
	ADD_FAILURE() << "fail() at\n" << qPrintable(backtrace) << "with a msg:\n" << qPrintable(errorMsg);
	context->throwError(QObject::tr("fail() with the message: %1 at\n%2").arg(errorMsg).arg(backtrace));
	return {};
}

QScriptValue scriptAddFailure(QScriptContext *context, QScriptEngine *engine)
{
	Q_UNUSED(engine);
	const QString backtrace = QStringList(context->backtrace().mid(1)).join("\n");
	if (context->argumentCount() > 1) {
		ADD_FAILURE() << "'add_failure()' must have exactly 0 or 1 argument";
		return {};
	}

	if (context->argumentCount() == 1) {
		if (!(context->argument(0).isValid() && context->argument(0).isString())) {
			ADD_FAILURE() << "incorrect add_failure() at\n" << qPrintable(backtrace);
			return {};
		}
	}

	const QString errorMsg = (context->argumentCount() == 1) ? context->argument(0).toString() : "";
	ADD_FAILURE() << "add_failure() at\n" << qPrintable(backtrace) << "with a msg:\n" << qPrintable(errorMsg);
	return {};
}

QScriptValue scriptExpect(QScriptContext *context, QScriptEngine *engine)
{
	Q_UNUSED(engine);
	const QString backtrace = QStringList(context->backtrace().mid(1)).join("\n");
	if (context->argumentCount() != 1) {
		ADD_FAILURE() << "'expect()' must have exactly one argument. Fail at\n" << qPrintable(backtrace);
		return {};
	}

	if (!context->argument(0).toBool()) {
		ADD_FAILURE() << "Unexpected value at\n" << qPrintable(backtrace);
		return {};
	}

	return {};
}

QRealGuiTests::QRealGuiTests()
	: mScriptFolderName("qrealScripts")
	, mTimeToExpose(7000)
	, mTimeLimit(67000)
{
}

void QRealGuiTests::SetUp()
{
	mWindow = start();
	if (mWindow == nullptr) {
		FAIL() << "Could not attach to main window.";
	}

	mWindow->setAttribute(Qt::WA_DeleteOnClose);
	QObject::connect(mWindow, &MainWindow::goingToBeDestroyed, [this] () { qInfo() << "MainWindow is closing..."; } );

	mScriptingControlAPI = mWindow->createScriptingControlAPI();
	if (mScriptingControlAPI == nullptr) {
		FAIL() << "mScriptingControlAPI is nullptr";
	}

	mScriptingControlAPI->registerNewFunction(scriptAssert, "assert");
	mScriptingControlAPI->registerNewFunction(scriptFail, "fail");
	mScriptingControlAPI->registerNewFunction(scriptAddFailure, "add_failure");
	mScriptingControlAPI->registerNewFunction(scriptExpect, "expect");

	// ATTENTION: workarounds!
	mScriptingControlAPI->registerNewFunction(closeExpectedDialog, "closeExpectedDialog");
	mScriptingControlAPI->registerNewFunction(chooseExpectedDialogDiagram, "chooseExpectedDialogDiagram");

	QScriptEngine *engine = mScriptingControlAPI->engine();
	mTestAgent = new TestAgent(engine);
	// engine->setAgent(mTestAgent); // For writting and debugging. Don't remove!

	mReturnCode = CRASHCODE;
	lambdaSingleShot(mTimeLimit, [this]() { failTest(); } );
}

// It may be usefull to use the LOG for some information about a failed/passed/running test
void QRealGuiTests::TearDown()
{
	delete mScriptingControlAPI;
	mScriptingControlAPI = nullptr;
	mCurrentIncludedFiles.clear();
	mCurrentTotalProgram.clear();
	mCurrentEvaluatingScript.clear();
	mCurrentEvaluatingScriptFile.clear();
	mScript.clear();
	mFileName.clear();
	mRelativeName.clear();
	mCommonScripts.clear();

	QLOG_INFO() << "------------------- APPLICATION FINISHED -------------------";
	QLOG_INFO() << "with exit code " << QString::number(mReturnCode);

	if (mReturnCode) {
		ADD_FAILURE() << "ERROR: returnCode of the last app = " << qPrintable(QString::number(mReturnCode));
	}
}

void QRealGuiTests::run(const QString &relativeFileName)
{
	includeCommonScript("common.js");
	const QString scriptDirName = PlatformInfo::applicationDirPath() +
			"/../../qrtest/unitTests/guiTests/testScripts/" + mScriptFolderName + "/";

	const QString fileName = QDir::cleanPath(scriptDirName) + "/" + relativeFileName;
	const QString script = readFile(fileName);
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

	lambdaSingleShot(mTimeToExpose, [this]() {
		if (::testing::Test::HasFailure()) {
			return;
		}

		for (const QString & scriptName : mCommonScripts) {
			runCommonScript(scriptName);
			if (::testing::Test::HasFailure()) {
				return;
			}
		}

		mCurrentEvaluatingScriptFile = mRelativeName;
		mCurrentEvaluatingScript = mScript;
		mCurrentTotalProgram += mScript;

		mScriptingControlAPI->evaluateScript(mScript, mFileName);
		checkLastEvaluating(mRelativeName);
		if (::testing::Test::HasFailure()) {
			return;
		}

		if (QApplication::activePopupWidget()) {
			QApplication::activePopupWidget()->close();
		}

		QApplication::closeAllWindows();
	} );

	mReturnCode = QApplication::exec();
}

void QRealGuiTests::includeCommonScript(const QString &relativeFileName)
{
	mCommonScripts << relativeFileName;
}

void QRealGuiTests::includeCommonScript(const QStringList &fileList)
{
	mCommonScripts += fileList;
}

void QRealGuiTests::failTest()
{
	mScriptingControlAPI->abortEvaluation();
	mScriptingControlAPI->engine()->currentContext()->throwError(QObject::tr("Test is failed because of freeze!"));
	exterminate(FREEZECODE);
}

QString QRealGuiTests::scriptFolderName() const
{
	return mScriptFolderName;
}

void QRealGuiTests::setScriptFolderName(const QString &folder)
{
	mScriptFolderName = folder;
}

int QRealGuiTests::timeToExpose() const
{
	return mTimeToExpose;
}

void QRealGuiTests::setTimeToExpose(const int timeToExpose)
{
	mTimeToExpose = timeToExpose;
}

int QRealGuiTests::timeLimit() const
{
	return mTimeLimit;
}

void QRealGuiTests::setTimeLimit(const int timeLimit)
{
	mTimeLimit = timeLimit;
}

void QRealGuiTests::runCommonScript(const QString &relativeFileName)
{
	const QString scriptDirName = PlatformInfo::applicationDirPath() +
			"/../../qrtest/unitTests/guiTests/testScripts/";
	const QString fileName = QDir::cleanPath(scriptDirName) + "/" + relativeFileName;
	const QString script = readFile(fileName);
	mCurrentEvaluatingScriptFile = relativeFileName;
	mCurrentIncludedFiles << fileName;
	mCurrentEvaluatingScript = script;
	mCurrentTotalProgram += script;
	if (::testing::Test::HasFailure()) {
		return;
	}

	checkScriptSyntax(script, relativeFileName);
	if (::testing::Test::HasFailure()) {
		return;
	}

	mScriptingControlAPI->evaluateScript(script, fileName);
	checkLastEvaluating(relativeFileName);
}

void QRealGuiTests::checkScriptSyntax(const QString &script, const QString &errorMsg)
{
	const QScriptSyntaxCheckResult checkResult = mScriptingControlAPI->checkSyntax(script);
	if (checkResult.state() != QScriptSyntaxCheckResult::Valid) {
		QApplication::quit();
		FAIL() << "Failed coz code is invalide\n" << qPrintable(checkResult.errorMessage())
				<< "\n" << qPrintable(errorMsg);
	}
}

void QRealGuiTests::checkLastEvaluating(const QString &errorMsg)
{
	if (mScriptingControlAPI->hasUncaughtException()) {
		const QString backtrace = mScriptingControlAPI->uncaughtExceptionBacktrace().join('\n');
		const QString exceptionMsg = mScriptingControlAPI->uncaughtException().toString();
		mScriptingControlAPI->clearExceptions();
		if (QApplication::activePopupWidget()) {
			QApplication::activePopupWidget()->close();
		}

		QApplication::closeAllWindows();
		FAIL() << "Failed. Uncaught exception of the last evaluating was thrown\n"
				<< qPrintable(backtrace) << "\n" << qPrintable(errorMsg) << "\n" << qPrintable(exceptionMsg) << "\n";
	}
}

QString QRealGuiTests::readFile(const QString &fileName) const
{
	QFile scriptFile(fileName);
	if (!scriptFile.open(QIODevice::ReadOnly)) {
		ADD_FAILURE() << "Can't open file for reading for gui test: " << qPrintable(fileName);
	}

	QTextStream stream(&scriptFile);
	const QString contents = stream.readAll();
	scriptFile.close();
	return contents;
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

//TEST_F(QRealGuiTests, dockWidgetsExistence)
//{
//	run("dockWidgetsExistence.js");
//}

TEST_F(QRealGuiTests, editActionsExistence)
{
	run("editActionsExistence.js");
}

TEST_F(QRealGuiTests, fileActionsExistence)
{
	run("fileActionsExistence.js");
}

TEST_F(QRealGuiTests, findDialogElementsExistence)
{
	run("findDialogElementsExistence.js");
}

TEST_F(QRealGuiTests, helpActionsExistence)
{
	run("helpActionsExistence.js");
}

TEST_F(QRealGuiTests, mouseGesturesElementsExistence)
{
	run("mouseGesturesElementsExistence.js");
}

TEST_F(QRealGuiTests, preferenceDialogElementsExistence)
{
	run("preferenceDialogElementsExistence.js");
}

TEST_F(QRealGuiTests, settingsActionsExistence)
{
	run("settingsActionsExistence.js");
}

TEST_F(QRealGuiTests, toolbarsElementsExistence)
{
	run("toolbarsElementsExistence.js");
}

TEST_F(QRealGuiTests, toolsActionsExistence)
{
	run("toolsActionsExistence.js");
}

//TEST_F(QRealGuiTests, viewActionsExistence)
//{
//	run("viewActionsExistence.js");
//}

TEST_F(QRealGuiTests, dockWidgetsFunctioning)
{
	run("dockWidgetsFunctioning.js");
}

TEST_F(QRealGuiTests, editActionsFunctioning)
{
	run("editActionsFunctioning.js");
}

TEST_F(QRealGuiTests, fileActionsFunctioning)
{
	run("fileActionsFunctioning.js");
}

TEST_F(QRealGuiTests, findDialogElementsFunctioning)
{
	run("findDialogElementsFunctioning.js");
}

TEST_F(QRealGuiTests, helpActionsFunctioning)
{
	run("helpActionsFunctioning.js");
}

TEST_F(QRealGuiTests, preferenceDialogElementsFunctioning)
{
	run("preferenceDialogElementsFunctioning.js");
}

TEST_F(QRealGuiTests, toolbarsElementsFunctioning)
{
	run("toolbarsElementsFunctioning.js");
}

TEST_F(QRealGuiTests, toolsActionsFunctioning)
{
	run("toolsActionsFunctioning.js");
}

TEST_F(QRealGuiTests, viewActionsFunctioning)
{
	run("viewActionsFunctioning.js");
}

TEST_F(QRealGuiTests, autoSaveWork)
{
	run("autoSaveWork.js");
}

TEST_F(QRealGuiTests, createDiagramInCurrentModel)
{
	run("createDiagramInCurrentModel.js");
}

TEST_F(QRealGuiTests, createRootElementOnSceneAndQuit)
{
	run("createRootElementOnSceneAndQuit.js");
}

//TEST_F(QRealGuiTests, fullscreenModeScript)
//{
//	run("fullscreenModeScript.js");
//}

TEST_F(QRealGuiTests, linkNodeCooperationScript)
{
	run("linkNodeCooperationScript.js");
}

TEST_F(QRealGuiTests, logicalGraphicalModelsAndSceneInteraction)
{
	run("logicalGraphicalModelsAndSceneInteraction.js");
}

TEST_F(QRealGuiTests, miniMapWorkScript)
{
	run("miniMapWorkScript.js");
}

TEST_F(QRealGuiTests, paletteAndEditorPropertiesConcordance)
{
	run("paletteAndEditorPropertiesConcordance.js");
}

TEST_F(QRealGuiTests, paletteSearchScript)
{
	run("paletteSearchScript.js");
}

TEST_F(QRealGuiTests, propertyEditorScript)
{
	run("propertyEditorScript.js");
}

TEST_F(QRealGuiTests, rightclickAllScreenScript)
{
	run("rightclickAllScreenScript.js");
}

TEST_F(QRealGuiTests, saveOpenScript)
{
	run("saveOpenScript.js");
}

TEST_F(QRealGuiTests, verySimpleShapeEditorTest)
{
	run("verySimpleShapeEditorTest.js");
}

//TEST_F(QRealGuiTests, creatingNewDiagrams)
//{
//	run("creatingNewDiagrams.js");
//}
