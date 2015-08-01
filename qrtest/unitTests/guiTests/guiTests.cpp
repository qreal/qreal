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

#include "guiTests.h"
#include "startQreal.cpp"

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

#include "dialogs/projectManagement/suggestToCreateProjectDialog.h"
#include "dialogs/projectManagement/suggestToCreateDiagramWidget.h"

using namespace guiTesting;
using namespace qReal;

// мб добавить assertnotnull для объектов в скрипте

QScriptValue scriptAssert(QScriptContext *context, QScriptEngine *engine)
{
	Q_UNUSED(engine);

	if (context->argumentCount() != 1) {
		ADD_FAILURE() << "'assert' shall have exactly one argument";
		return {};
	}

	if (!context->argument(0).toBool()) {
		ADD_FAILURE() << "Fail at\n" // FAIL() должен быть, можно бросать исключение!!!
				<< QStringList(context->backtrace().mid(1)).join("\n").toStdString();
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
	}
	// TODO: надо аборт написать и закрыть все окна, выйти из приложения.
	return {};
}

//QScriptValue scriptExist(QScriptContext *context, QScriptEngine *engine) // q_script_cast?
//{
//	Q_UNUSED(engine);

//	if (context->argumentCount() != 1) {
//		ADD_FAILURE() << "'exist' shall have exactly one argument";
//		return {};
//	}

//	if (context->argument(0).isNull()) {
//		ADD_FAILURE() << "Fail at\n" // FAIL() должен быть
//				<< QStringList(context->backtrace().mid(1)).join("\n").toStdString();
//	}

//	return {};
//}

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
	}

	return {};
}

//QScriptValue scriptExpectIsNotNull(QScriptContext *context, QScriptEngine *engine)
//{
//	Q_UNUSED(engine);

//	if (context->argumentCount() != 1) {
//		ADD_FAILURE() << "'expectNotNull' shall have exactly one argument";
//		return {};
//	}

//	if (context->argument(0).isNull()) {
//		ADD_FAILURE() << "Expecting failure at\n"
//				<< QStringList(context->backtrace().mid(1)).join("\n").toStdString();
//	}

//	return {};
//}

// mainwindow, имя диалога, название диаграммы, время
QScriptValue scriptExpectDialog(QScriptContext *context, QScriptEngine *engine)
{
	Q_UNUSED(engine);

	if (context->argumentCount() != 4) {
		ADD_FAILURE() << "'expectDialog' shall have exactly 4 arguments";
		return {};
	}

	if (!(context->argument(0).isValid() && !context->argument(0).isNull())) {
		ADD_FAILURE() << "Assertion failure at\n"
				<< QStringList(context->backtrace().mid(1)).join("\n").toStdString();
	}

	if (!(context->argument(1).isValid() && context->argument(1).isString())) {
		ADD_FAILURE() << "Assertion failure at\n"
				<< QStringList(context->backtrace().mid(1)).join("\n").toStdString();
	}

	if (!(context->argument(2).isValid() && context->argument(2).isString())) {
		ADD_FAILURE() << "Assertion failure at\n"
				<< QStringList(context->backtrace().mid(1)).join("\n").toStdString();
	}

	if (!(context->argument(3).isValid() && context->argument(3).isNumber())) {
		ADD_FAILURE() << "Assertion failure at\n"
				<< QStringList(context->backtrace().mid(1)).join("\n").toStdString();
	}

	QString diagramName = context->argument(2).toString();
	QString dialogTitle = context->argument(1).toString();
	int mces = context->argument(3).toInt32();
	MainWindow *mainWindow = qobject_cast<MainWindow *>(context->argument(0).toQObject());
	EXPECT_TRUE(mainWindow != nullptr); // тут должен быть фейл
	QTimer::singleShot(mces, [=]() { // возможно, фейлы тут работают не так, как должны
	// TODO: надо переделать for, дописать переменные понятные, добавить проверок (ассертов)
		EXPECT_GT(mces, 0);
		ASSERT_TRUE(mainWindow != NULL);
		QList<QWidget *> allDialogs = mainWindow->findChildren<QWidget *>(); // вообще это работает, но хочется без списка, а найти "первого" встречного, попросить помочь реализовать
		ASSERT_FALSE(allDialogs.isEmpty());
		for (int i = 0; i < allDialogs.length(); ++i) {
			if (allDialogs.at(i)->windowTitle() == dialogTitle) {
				SuggestToCreateProjectDialog *listDialog = dynamic_cast <SuggestToCreateProjectDialog *>(allDialogs.at(i));
				ASSERT_TRUE(listDialog != NULL);
				QList<SuggestToCreateDiagramWidget *> listWidget = listDialog->findChildren<SuggestToCreateDiagramWidget *>();
				ASSERT_FALSE(listWidget.isEmpty());
				for (int k = 0; k < listWidget.length(); ++k) {
					if (listWidget.at(k) != nullptr) {
						SuggestToCreateDiagramWidget *suggestWidget = listWidget.at(k);
						for (int m = 0; m < suggestWidget->getQListWidget()->count(); ++m) {
							if (suggestWidget->getQListWidget()->item(m)->text() == diagramName) {
								suggestWidget->getQListWidget()->itemDoubleClicked(suggestWidget->getQListWidget()->item(m));
								break;

								/*QPoint pos = listWidget.at(k)->visualItemRect(listWidget.at(k)->item(m)).topLeft();
								QMouseEvent *mouseEvent = new QMouseEvent(QEvent::MouseButtonDblClick,
															  pos,
															  Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
								QApplication::postEvent(listWidget.at(k)->getMainWidget()->parent(), mouseEvent);*/
							}
							if (m == suggestWidget->getQListWidget()->count() - 1) {
								FAIL() << "doesnt exist " << diagramName.toStdString() << " diagram";
								// TODO: в подобных местах надо закрывать диалог, чтоб не зависало. можно в самом начали сделать сингшот на закрытие всех окон приложения.
							}
						}
						break;
					}
				}
				break;
			}
			if (i == allDialogs.length() - 1) {
				FAIL() << "doesnt exist " << dialogTitle.toStdString() << " dialog";
			}
		}
	});

	return {};
}

QScriptValue scriptCloseExpectedDialog(QScriptContext *context, QScriptEngine *engine)
{
	Q_UNUSED(engine);

	if (context->argumentCount() != 3) {
		ADD_FAILURE() << "'expectDialog' shall have exactly 4 arguments";
		return {};
	}

	if (!(context->argument(0).isValid() && !context->argument(0).isNull())) {
		ADD_FAILURE() << "Assertion failure at\n"
				<< QStringList(context->backtrace().mid(1)).join("\n").toStdString();
	}

	if (!(context->argument(1).isValid() && context->argument(1).isString())) {
		ADD_FAILURE() << "Assertion failure at\n"
				<< QStringList(context->backtrace().mid(1)).join("\n").toStdString();
	}

	if (!(context->argument(2).isValid() && context->argument(2).isNumber())) {
		ADD_FAILURE() << "Assertion failure at\n"
				<< QStringList(context->backtrace().mid(1)).join("\n").toStdString();
	}

	QString dialogTitle = context->argument(1).toString();
	int mces = context->argument(2).toInt32();
	MainWindow *mainWindow = qobject_cast<MainWindow *>(context->argument(0).toQObject());
	EXPECT_TRUE(mainWindow != nullptr);
	QTimer::singleShot(mces, [=]() {
		EXPECT_GT(mces, 0);
		ASSERT_TRUE(mainWindow != NULL);
		QList<QWidget *> allDialogs = mainWindow->findChildren<QWidget *>();
		ASSERT_FALSE(allDialogs.isEmpty());
		for (int i = 0; i < allDialogs.length(); ++i) {
			if (allDialogs.at(i)->windowTitle() == dialogTitle) {
				allDialogs.at(i)->close();
				break;
			}
			if (i == allDialogs.length() - 1) {
				FAIL() << "doesnt exist " << dialogTitle.toStdString() << " dialog";
			}
		}
	});

	return {};
}

void wait(int duration) // а надо ли копипастить?
{
	QEventLoop eventLoop;
	if (duration != -1) {
		QTimer::singleShot(duration, &eventLoop, SLOT(quit()));
	}

	eventLoop.exec();
}

void guiTests::SetUp() // возможно эти строчки следует поместить для сетапа для всех тестов
{
//	QString program = QApplication::applicationFilePath() + "/../qreal-d.exe";
//	QStringList arguments;
//	QProcess *qrealInstance = new QProcess();
//	// qrealInstance->waitForStarted(30000); // надо ли блокировать сигналы?
//	qrealInstance->start(program, arguments);

	int argc = 1;
	char *argv[] = {"C:/Users/Kirill/Desktop/qreal/bin/debug/guiTests-d.exe", "\0"};

//	qDebug() << QApplication::instance() << "сетап";
//	if (QApplication::instance() == nullptr) {
//		QApplication app(argc, argv);
//		qDebug() << "я внутри в сетапе" << QApplication::instance();
//	}
	MainWindow *window = start(argc, argv);
	//window->activateWindow();
	//QApplication::setActiveWindow(window);
	mWindow = window;
	mWindow->setAttribute(Qt::WA_DeleteOnClose);
//mApp = qrealPointer;
	//wait(10000);
	mMainWindowScriptAPIInterface = dynamic_cast<MainWidnowScriptAPIInterface *>(window);
	//if (mMainWidnowScriptAPIInterface == nullptr) {
	//	ADD_FAILURE() << "MainWindow is not found"; // можно ли сюда добавлять ассерты? (в сетап)
	//}
	//qrealInstance->dumpObjectTree();
	//while(qrealInstance->state() != 2){}; // костыль, надо бы и время учитывать
	// if (!qrealInstance->waitForStarted(10000)) {
	//	// как-то обработать (на случай если программа не запустилась или путь неверный)
	//}
//	mQrealInstance = qrealInstance;

	// app.setAttribute(Qt::AA_Use96Dpi, true); // может понадобится на Xdisplay?
	mMainWindowScriptAPIInterface->registerNewFunction(scriptAssert, "assert");
	mMainWindowScriptAPIInterface->registerNewFunction(scriptAddFailure, "failure");
//	mMainWidnowScriptAPIInterface->registerNewFunction(scriptExist, "exist");
	mMainWindowScriptAPIInterface->registerNewFunction(scriptExpect, "expect");
	mMainWindowScriptAPIInterface->registerNewFunction(scriptCloseExpectedDialog, "closeExpectedDialog"); // это особенная функция, содержащая как дейтсвие, так и проверки для диалогов TODO: то же самое, что и лля другой
//	mMainWidnowScriptAPIInterface->registerNewFunction(scriptExpectIsNotNull, "expectNotNull");
	mMainWindowScriptAPIInterface->registerNewFunction(scriptExpectDialog, "chooseExpectedDialogDiagram"); // это особенная функция, содержащая как дейтсвие, так и проверки для диалогов
}

void guiTests::TearDown() // возможно стоит смотреть информацию с логов и что-либо там делать.
// а для скриптов можно выполнять действия с известными моделями типа езды по линии.
{
//	delete mWindow;
	  // или deletelater?
//	delete mMainWidnowScriptAPIInterface;

	//mApp->exit();
//	mQrealInstance->terminate();
	// ниже надо написать блок об обработке ошибки выхода
//	if (mQrealInstance->exitStatus() != QProcess::NormalExit) {
//		ADD_FAILURE() << "Error code of creshed process: " << mQrealInstance->error(); // можно ли писать fail в teardown
//	}
//	delete mQrealInstance; // или deletelater?
}

void guiTests::run(const QString &script)
{
	QScriptSyntaxCheckResult checkResult = mMainWindowScriptAPIInterface->checkSyntax(script);
	if (checkResult.state() != QScriptSyntaxCheckResult::Valid) {
		QApplication::closeAllWindows();
		FAIL() << "Failed coz code is invalide\n" << checkResult.errorMessage().toStdString();
	}
	QTimer::singleShot(8000, [=]() { // возможно, фейлы тут работают не так, как должны
			mMainWindowScriptAPIInterface->evaluateScript(script);
			if (mMainWindowScriptAPIInterface->hasUncaughtException()) {
				std::string backtrace = mMainWindowScriptAPIInterface->uncaughtExceptionBacktrace().join('\n').toStdString();
				mMainWindowScriptAPIInterface->clearExceptions();
				QApplication::closeAllWindows();
				FAIL() << "Failed coz uncaughtException of the last evaluating exists\n" << backtrace;
			}

		QApplication::closeAllWindows();
	//	QApplication::quit();
	});
	QApplication::exec();
}

void guiTests::runFromFile(const QString &fileName1)
{	
	QString fileName = QApplication::applicationFilePath() +
			"/../../../qrtest/unitTests/guiTests/testScripts/qrealScripts/" + fileName1;
	QFile scriptFile(fileName);
	if (!scriptFile.open(QIODevice::ReadOnly)) {
		// handle error
		// qDebug() << "Cant open file for reading for gui test: " << fileName.toStdString(); // может убрать?
		ADD_FAILURE() << "Cant open file for reading for gui test: " << fileName.toStdString();
	}
	QTextStream stream(&scriptFile);
	QString contents = stream.readAll();
	scriptFile.close();
	run(contents);
}

TEST_F(guiTests, sanityCheck)
{
	run("assert(true);");
	ASSERT_EQ(2, 1 + 1);
}

TEST_F(guiTests, editActionsExistence)
{
//	runFromFile("editActionsExistence.js"); // мб qs?
	run("var mainWindow = api.ui().mainWindow();assert(mainWindow != null);api.changeWindow(mainWindow);api.wait(500);var newProject = api.ui().widget(\"QToolButton\", \"actionNewProject\");assert(newProject != null);closeExpectedDialog(mainWindow, \"Create project\", 3000);	api.cursor().moveTo(newProject, 400);api.cursor().leftButtonPress(newProject);api.cursor().leftButtonRelease(newProject, 400);");
}

TEST_F(guiTests, viewActionsExistence)
{
	runFromFile("editActionsExistence.js");
//	runFromFile("viewActionsExistence.js");
}

//TEST_F(guiTests, findDialogElementsExistence)
//{
//	runFromFile("findDialogElementsExistence.js");
//}

//TEST_F(guiTests, helpActionsExistence)
//{
//	runFromFile("helpActionsExistence.js");
//}

//TEST_F(guiTests, mainPanelsElementsExistence)
//{
//	runFromFile("mainPanelsElementsExistence.js");
//}

//TEST_F(guiTests, mouseGesturesElementsExistence)
//{
//	runFromFile("mouseGesturesElementsExistence.js");
//}

//TEST_F(guiTests, preferenceDialogElementsExistence)
//{
//	runFromFile("preferenceDialogElementsExistence.js");
//}

//TEST_F(guiTests, settingsActionsExistence)
//{
//	runFromFile("settingsActionsExistence.js");
//}

//TEST_F(guiTests, tabSceneExistence)
//{
//	runFromFile("tabSceneExistence.js");
//}

//TEST_F(guiTests, toolbarsElementsExistence)
//{
//	runFromFile("toolbarsElementsExistence.js");
//}

//TEST_F(guiTests, toolsActionsExistence)
//{
//	runFromFile("toolsActionsExistence.js");
//}

//TEST_F(guiTests, fileActionsExistence)
//{
//	runFromFile("fileActionsExistence.js");
//}

//TEST_F(guiTests, fileActionsFunctioning)
//{
//	runFromFile("fileActionsFunctioning.js");
//}

//TEST_F(guiTests, findDialogElementsFunctioning)
//{
//	runFromFile("findDialogElementsFunctioning.js");
//}

//TEST_F(guiTests, helpActionsFunctioning)
//{
//	runFromFile("helpActionsFunctioning.js");
//}

//TEST_F(guiTests, mainPanelsElementsFunctioning)
//{
//	runFromFile("mainPanelsElementsFunctioning.js");
//}

//TEST_F(guiTests, mouseGesturesElementsFunctioning)
//{
//	runFromFile("mouseGesturesElementsFunctioning.js");
//}

//TEST_F(guiTests, preferenceDialogElementsFunctioning)
//{
//	runFromFile("preferenceDialogElementsFunctioning.js");
//}

//TEST_F(guiTests, settingsActionsFunctioning)
//{
//	runFromFile("settingsActionsFunctioning.js");
//}

//TEST_F(guiTests, tabSceneFunctioning)
//{
//	runFromFile("tabSceneFunctioning.js");
//}

//TEST_F(guiTests, toolbarsElementsFunctioning)
//{
//	runFromFile("toolbarsElementsFunctioning.js");
//}

//TEST_F(guiTests, toolsActionsFunctioning)
//{
//	runFromFile("toolsActionsFunctioning.js");
//}

//TEST_F(guiTests, viewActionsFunctioning)
//{
//	runFromFile("viewActionsFunctioning.js");
//}

//TEST_F(guiTests, editActionsFunctioning)
//{
//	runFromFile("editActionsFunctioning.js");
//}

//TEST_F(guiTests, hotKeysExistanceAndFunctioning)
//{
//	runFromFile("hotKeysExistanceAndFunctioning.js");
//}

//TEST_F(guiTests, script1)
//{
//	runFromFile("script1.js");
//}

//TEST_F(guiTests, script2)
//{
//	runFromFile("script2.js");
//}

//TEST_F(guiTests, script3)
//{
//	runFromFile("script3.js");
//}

//TEST_F(guiTests, script4)
//{
//	runFromFile("script4.js");
//}

//TEST_F(guiTests, script5)
//{
//	runFromFile("script5.js");
//}

//TEST_F(guiTests, script6)
//{
//	runFromFile("script6.js");
//}

//TEST_F(guiTests, script7)
//{
//	runFromFile("script7.js");
//}

//TEST_F(guiTests, script8)
//{
//	runFromFile("script8.js");
//}

//TEST_F(guiTests, script9)
//{
//	runFromFile("script9.js");
//}

//TEST_F(guiTests, script10)
//{
//	runFromFile("script10.js");
//}
