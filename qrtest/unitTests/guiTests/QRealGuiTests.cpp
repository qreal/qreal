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

#include "QRealGuiTests.h"
#include "startQreal.cpp"
#include "dialogs/projectManagement/suggestToCreateProjectDialog.h"
#include "dialogs/projectManagement/suggestToCreateDiagramWidget.h"

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

QScriptValue scriptAssert(QScriptContext *context, QScriptEngine *engine)
{
	Q_UNUSED(engine);

	if (context->argumentCount() != 1) {
		ADD_FAILURE() << "'assert' shall have exactly one argument";
		return {};
	}

	if (!context->argument(0).toBool()) {
		ADD_FAILURE() << "Fail at\n" // FAIL() должен быть, можно бросать исключение!!! // TODO! надо проверить, что будет, если просто бросать исключения. как тесты пойдут
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

	return {}; // что делает этот код?
}


// mainwindow, имя диалога, время
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
	QTimer::singleShot(mces, [=]() {
		EXPECT_GT(mces, 0);
		ASSERT_TRUE(mainWindow != nullptr);
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

void QRealGuiTests::SetUp()
{
	mReturnCode = CRASHCODE;

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
	mainWindowScriptAPIInterface->registerNewFunction(scriptExpect, "expect");
	mainWindowScriptAPIInterface->registerNewFunction(scriptCloseExpectedDialog, "closeExpectedDialog"); // это особенная функция, содержащая как дейтсвие, так и проверки для диалогов TODO: то же самое, что и лля другой
	mainWindowScriptAPIInterface->registerNewFunction(scriptExpectDialog, "chooseExpectedDialogDiagram"); // это особенная функция, содержащая как дейтсвие, так и проверки для диалогов
}

void QRealGuiTests::TearDown() // возможно стоит смотреть информацию с логов и что-либо там делать.
{
	SettingsManager::setValue("scriptInterpretation", mScriptInterpretationDefaultValue);
	QLOG_INFO() << "------------------- APPLICATION FINISHED -------------------";
	if (mReturnCode) {
		FAIL() << "Failed coz returnCode of app = \n" << std::to_string(mReturnCode);
	}
}

void QRealGuiTests::run(const QString &script) // мб добавить функцию максимального ожидания сюда или вообще?
{
	QScriptSyntaxCheckResult checkResult = mainWindowScriptAPIInterface->checkSyntax(script);
	if (checkResult.state() != QScriptSyntaxCheckResult::Valid) {
		QApplication::closeAllWindows();
		FAIL() << "Failed coz code is invalide\n" << checkResult.errorMessage().toStdString();
	}
	QTimer::singleShot(8000, [=]() { // возможно, фейлы тут работают не так, как должны
			mainWindowScriptAPIInterface->evaluateScript(script);
			if (mainWindowScriptAPIInterface->hasUncaughtException()) {
				std::string backtrace = mainWindowScriptAPIInterface->uncaughtExceptionBacktrace().join('\n').toStdString();
				mainWindowScriptAPIInterface->clearExceptions();
				QApplication::closeAllWindows();
				FAIL() << "Failed coz uncaughtException of the last evaluating exists\n" << backtrace;
			}

		QApplication::closeAllWindows();
	});
	mReturnCode = QApplication::exec();
}

void QRealGuiTests::runFromFile(const QString &relativeFileName)
{
	QString scriptDirName = QApplication::applicationFilePath() +
			"/../../../qrtest/unitTests/guiTests/testScripts/" + scriptFolderName + "/";
	QString fileName = QDir::cleanPath(scriptDirName) + "/" + relativeFileName;

//	QString fileName; // этот код пока что можно оставить, так как вдруг теневая сборка.
//	QDir scriptDir(scriptDirName);
//	if (scriptDir.exists()) {
//		fileName = scriptDirName + relativeFileName;
//	} else {
//		fileName = QApplication::applicationDirPath() + "/testScripts/qrealScripts/" + relativeFileName;
//	}

	QFile scriptFile(fileName);
	if (!scriptFile.open(QIODevice::ReadOnly)) {
		ADD_FAILURE() << "Cant open file for reading for gui test: " << fileName.toStdString();
	}
	QTextStream stream(&scriptFile);
	QString contents = stream.readAll();
	scriptFile.close();
	run(contents);
}

//TEST_F(QRealGuiTests, sanityCheck)
//{
//	run("assert(true);");
//	ASSERT_EQ(2, 1 + 1);
//}

//TEST_F(QRealGuiTests, editActionsExistence)
//{
//	runFromFile("editActionsExistence.js"); // мб qs?
//}

//TEST_F(QRealGuiTests, viewActionsExistence)
//{
//	runFromFile("viewActionsExistence.js");
//}

//TEST_F(QRealGuiTests, findDialogElementsExistence)
//{
//	runFromFile("findDialogElementsExistence.js");
//}

//TEST_F(QRealGuiTests, helpActionsExistence)
//{
//	runFromFile("helpActionsExistence.js");
//}

//TEST_F(QRealGuiTests, mainPanelsElementsExistence)
//{
//	runFromFile("mainPanelsElementsExistence.js");
//}

//TEST_F(QRealGuiTests, mouseGesturesElementsExistence)
//{
//	runFromFile("mouseGesturesElementsExistence.js");
//}

//TEST_F(QRealGuiTests, preferenceDialogElementsExistence)
//{
//	runFromFile("preferenceDialogElementsExistence.js");
//}

//TEST_F(QRealGuiTests, settingsActionsExistence)
//{
//	runFromFile("settingsActionsExistence.js");
//}

//TEST_F(QRealGuiTests, tabSceneExistence)
//{
//	runFromFile("tabSceneExistence.js");
//}

//TEST_F(QRealGuiTests, toolbarsElementsExistence)
//{
//	runFromFile("toolbarsElementsExistence.js");
//}

//TEST_F(QRealGuiTests, toolsActionsExistence)
//{
//	runFromFile("toolsActionsExistence.js");
//}

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

//TEST_F(QRealGuiTests, script1)
//{
//	runFromFile("script1.js");
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
