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

#include <gtest/gtest.h>

#include <dialogs/projectManagement/suggestToCreateProjectDialog.h>
#include <dialogs/projectManagement/suggestToCreateDiagramWidget.h>
#include <./qrgui/mainWindow/mainWindow.h>

#include <QtTest/QTest>
#include <QtCore/QTimer>
#include <QtCore/QDebug>
#include <QtWidgets/QListWidget>

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
	const QString dialogTitle = context->argument(1).toString();
	const int mces = context->argument(2).toInt32();
	MainWindow * const mainWindow = qobject_cast<MainWindow *>(context->argument(0).toQObject());
	QTimer::singleShot(mces, [=]() {
		QList<QDialog *> allDialogs = mainWindow->findChildren<QDialog *>();
		for (QDialog * const dialog : allDialogs) {
			if (dialog->windowTitle() == dialogTitle || dialog->objectName() == dialogTitle) {
				dialog->close(); // here gui workaround
				return;
			}
		}
	});

	return {};
}

// Structure of the object: mainwindow->dialog->widgetList->diagrams. Looks in depth.
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

	const QString diagramName = context->argument(2).toString();
	const QString dialogTitle = context->argument(1).toString();
	const int mces = context->argument(3).toInt32();
	MainWindow * const mainWindow = qobject_cast<MainWindow *>(context->argument(0).toQObject());
	QTimer::singleShot(mces, [=]() {
		EXPECT_GT(mces, 0);
		ASSERT_TRUE(mainWindow != nullptr);
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
						for (int m = 0; m < suggestWidget->mainListWidget()->count(); ++m) {
							if (suggestWidget->mainListWidget()->item(m)->text() == diagramName) {
/*here gui workaround*/			suggestWidget->mainListWidget()->itemDoubleClicked(suggestWidget->mainListWidget()->item(m));
								break;
							}

							if (m == suggestWidget->mainListWidget()->count() - 1) {
								FAIL() << "Doesn't exist " << diagramName.toStdString() << " diagram";
							}
						}

						break;
					}
				}

				break;
			}

			if (i == allDialogs.length() - 1) {
				FAIL() << "Doesn't exist " << dialogTitle.toStdString() << " dialog";
			}
		}
	} );

	return {};
}
