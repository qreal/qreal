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

#pragma once

#include <QTest>

#include <gtest/gtest.h>
#include <qrutils/widgetFinder.h>

#include "QRealGuiTests.h"
#include "dialogs/projectManagement/suggestToCreateProjectDialog.h"
#include "dialogs/projectManagement/suggestToCreateDiagramWidget.h"
#include "qrgui/mainWindow/qrealApplication.h"
#include "qrgui/plugins/toolPluginInterface/usedInterfaces/mainWindowScriptAPIInterface.h"
#include "./qrgui/mainWindow/mainWindow.h"

#include <QRadioButton>
#include <QCheckBox>
#include <QComboBox>
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

/// @note This method works with a keyboard (not a mouse)
QScriptValue scriptActivateContextMenuAction(QScriptContext *context, QScriptEngine *engine)
{
	Q_UNUSED(engine);

	if (context->argumentCount() != 2) {
		ADD_FAILURE() << "'scriptActivateContextMenuAction' shall have exactly 2 arguments";
		return {};
	}

	if (!(context->argument(0).isValid() && context->argument(0).isString())) {
		ADD_FAILURE() << "Assertion failure at\n"
				<< QStringList(context->backtrace().mid(1)).join("\n").toStdString();
		return {};
	}

	if (!(context->argument(1).isValid() && context->argument(1).isNumber())) {
		ADD_FAILURE() << "Assertion failure at\n"
				<< QStringList(context->backtrace().mid(1)).join("\n").toStdString();
		return {};
	}

	QString actionName = context->argument(0).toString();
	int mces = context->argument(1).toInt32();
	QTimer::singleShot(mces, [=]() {
		EXPECT_GT(mces, 0);
		QMenu *contextMenu = dynamic_cast<QMenu *>(QApplication::activePopupWidget());
		QTest::keyClick(contextMenu, Qt::Key_Down);
		ASSERT_TRUE(contextMenu != nullptr);
		QList<QAction *> actions = contextMenu->actions();
		QAction *neededAction = nullptr;
		for (QAction *action: actions) {
			if (action->objectName() == actionName || action->text() == actionName) {
				neededAction = action;
			}
		}
		ASSERT_TRUE(neededAction != nullptr);
		for (const QAction *action : actions) {
			if (action == neededAction) {
				QTest::keyClick(contextMenu, Qt::Key_Enter);
				return;
			}
			if (!action->isSeparator()) {
				QTest::qWait(50);
				QTest::keyClick(contextMenu, Qt::Key_Down);
			}
		}
	});

	return {};
}

/// Closes popup context menu
///  @note This method works with a keyboard (not a mouse)
QScriptValue scriptCloseContextMenu(QScriptContext *context, QScriptEngine *engine)
{
	Q_UNUSED(engine);

	if (context->argumentCount() != 1) {
		ADD_FAILURE() << "'scriptCloseContextMenu' shall have exactly 1 arguments";
		return {};
	}

	if (!(context->argument(0).isValid() && context->argument(0).isNumber())) {
		ADD_FAILURE() << "Assertion failure at\n"
				<< QStringList(context->backtrace().mid(1)).join("\n").toStdString();
		return {};
	}

	int mces = context->argument(0).toInt32();
	QTimer::singleShot(mces, [=]() {
		EXPECT_GT(mces, 0);
		QMenu *contextMenu = dynamic_cast<QMenu *>(QApplication::activePopupWidget());
		QTest::keyClick(contextMenu, Qt::Key_Down);
		ASSERT_TRUE(contextMenu != nullptr);
		QTest::keyClick(contextMenu, Qt::Key_Escape);
	});

	return {};
}
