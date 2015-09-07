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

#include <QTest>
#include <gtest/gtest.h>

#include "dialogs/projectManagement/suggestToCreateProjectDialog.h"
#include "dialogs/projectManagement/suggestToCreateDiagramWidget.h"
#include "./../../../qrgui/mainWindow/mainWindow.h"

#include <QtScript/QScriptContext>
#include <QtScript/QScriptEngine>
#include <QtScript/QScriptValue>

#include <QTimer>
#include <QDebug>
#include <QListWidget>

#include "workaroundTestFunctions.h"

using namespace qReal;
using namespace guiTesting::workarounds;

QScriptValue guiTesting::workarounds::reachedEndOfScript(QScriptContext *context, QScriptEngine *engine)
{
	Q_UNUSED(engine);

	if (context->argumentCount() != 0) {
		ADD_FAILURE() << "'reachedEndOfScript' shall not have any argument";
		return {};
	}
	SettingsManager::setValue("reachedEndOfFile", "true"); // here gui workaround
	return {};
}

QScriptValue guiTesting::workarounds::closeExpectedDialog(QScriptContext *context
																  , QScriptEngine *engine)
{
	Q_UNUSED(engine);
	QString dialogTitle = context->argument(1).toString();
	int mces = context->argument(2).toInt32();
	MainWindow *mainWindow = qobject_cast<MainWindow *>(context->argument(0).toQObject());
	QTimer::singleShot(mces, [=]() {
		QList<QDialog *> allDialogs = mainWindow->findChildren<QDialog *>();
		for (QDialog * dialog : allDialogs) {
			if (dialog->windowTitle() == dialogTitle || dialog->objectName() == dialogTitle) {
				dialog->close(); // here gui workaround
				qDebug() << "dialog closed";
				return;
			}
		}
	});

	return {};
}

QScriptValue guiTesting::workarounds::chooseExpectedDialogDiagram(QScriptContext *context
																		 , QScriptEngine *engine)
{
	Q_UNUSED(engine);

	if (context->argumentCount() != 4) {
		ADD_FAILURE() << "'expectDialog' shall have exactly 4 arguments";
		return {};
	}

	if (!(context->argument(0).isValid() && !context->argument(0).isNull())) {
		ADD_FAILURE() << "Assertion failure at\n"
				<< QStringList(context->backtrace().mid(1)).join("\n").toStdString();
		return {};
	}

	for (int i = 1; i <= 2; ++i) {
		if (!(context->argument(i).isValid() && context->argument(i).isString())) {
			ADD_FAILURE() << "Assertion failure at\n"
					<< QStringList(context->backtrace().mid(1)).join("\n").toStdString();
			return {};
		}
	}

	if (!(context->argument(3).isValid() && context->argument(3).isNumber())) {
		ADD_FAILURE() << "Assertion failure at\n"
				<< QStringList(context->backtrace().mid(1)).join("\n").toStdString();
		return {};
	}

	QString diagramName = context->argument(2).toString();
	QString dialogTitle = context->argument(1).toString();
	int mces = context->argument(3).toInt32();
	MainWindow *mainWindow = qobject_cast<MainWindow *>(context->argument(0).toQObject());
	QTimer::singleShot(mces, [=]() {
		EXPECT_GT(mces, 0);
		ASSERT_TRUE(mainWindow != NULL);
		QList<QWidget *> allDialogs = mainWindow->findChildren<QWidget *>();
		ASSERT_FALSE(allDialogs.isEmpty());
		for (int i = 0; i < allDialogs.length(); ++i) {
			if (allDialogs.at(i)->windowTitle() == dialogTitle || allDialogs.at(i)->objectName() == dialogTitle) {
				SuggestToCreateProjectDialog *listDialog = dynamic_cast <SuggestToCreateProjectDialog *>(allDialogs.at(i));
				ASSERT_TRUE(listDialog != NULL);
				QList<SuggestToCreateDiagramWidget *> listWidget = listDialog->findChildren<SuggestToCreateDiagramWidget *>();
				ASSERT_FALSE(listWidget.isEmpty());
				for (int k = 0; k < listWidget.length(); ++k) {
					if (listWidget.at(k) != nullptr) {
						SuggestToCreateDiagramWidget *suggestWidget = listWidget.at(k);
						for (int m = 0; m < suggestWidget->getQListWidget()->count(); ++m) {
							if (suggestWidget->getQListWidget()->item(m)->text() == diagramName) {
/*here gui workaround*/			suggestWidget->getQListWidget()->itemDoubleClicked(suggestWidget->getQListWidget()->item(m));
								break;

								/*QPoint pos = listWidget.at(k)->visualItemRect(listWidget.at(k)->item(m)).topLeft();
								QMouseEvent *mouseEvent = new QMouseEvent(QEvent::MouseButtonDblClick,
															  pos,
															  Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
								QApplication::postEvent(listWidget.at(k)->getMainWidget()->parent(), mouseEvent);*/
							}
							if (m == suggestWidget->getQListWidget()->count() - 1) {
								FAIL() << "doesnt exist " << diagramName.toStdString() << " diagram";
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
