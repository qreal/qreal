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

#include <QtWidgets/QFileDialog>
#include <qrutils/qRealFileDialog.h>

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

//// mainwindow, имя диалога, время
//QScriptValue scriptCloseExpectedOpenSaveDialog(QScriptContext *context, QScriptEngine *engine)
//{
//	Q_UNUSED(engine);

//	if (context->argumentCount() != 3) { // можно в этом месте делать -1 аргемент и развилку для топлвлдиалога
//		ADD_FAILURE() << "'scriptCloseExpectedOpenSaveDialog' shall have exactly 3 arguments";
//		return {};
//	}

//	if (!(context->argument(0).isValid() && !context->argument(0).isNull())) {
//		ADD_FAILURE() << "Assertion failure at\n"
//				<< QStringList(context->backtrace().mid(1)).join("\n").toStdString();
//		return {};
//	}

//	if (!(context->argument(1).isValid() && context->argument(1).isString())) {
//		ADD_FAILURE() << "Assertion failure at\n"
//				<< QStringList(context->backtrace().mid(1)).join("\n").toStdString();
//		return {};
//	}

//	if (!(context->argument(2).isValid() && context->argument(2).isNumber())) {
//		ADD_FAILURE() << "Assertion failure at\n"
//				<< QStringList(context->backtrace().mid(1)).join("\n").toStdString();
//		return {};
//	}

//	QString dialogTitle = context->argument(1).toString();
//	int mces = context->argument(2).toInt32();
//	MainWindow *mainWindow = qobject_cast<MainWindow *>(context->argument(0).toQObject());
//	QTimer::singleShot(mces, [=]() {
//		EXPECT_GT(mces, 0);
//		ASSERT_TRUE(mainWindow != nullptr);
//		QList<QDialog *> allDialogs = mainWindow->findChildren<QDialog *>();
//		ASSERT_FALSE(allDialogs.isEmpty());
//		for (QDialog * dialog : allDialogs) {
//			if (dialog->windowTitle() == dialogTitle || dialog->objectName() == dialogTitle) {
//				QFileDialog *fileDialog = dynamic_cast<QFileDialog *>(dialog);
//				ASSERT_TRUE(fileDialog != nullptr);
//				fileDialog->close();
//				return;
//			}
//		}
//		FAIL() << "doesnt exist " << dialogTitle.toStdString() << " QFileDialog";
//	});

//	return {};
//}
//// mainwindow, имя диалога, имя файла, время
//QScriptValue scriptSaveFileInExpectedSaveDialog(QScriptContext *context, QScriptEngine *engine)
//{
//	Q_UNUSED(engine);

//	if (context->argumentCount() != 4) { // можно в этом месте делать -1 аргемент и развилку для топлвлдиалога
//		ADD_FAILURE() << "'scriptCloseExpectedOpenSaveDialog' shall have exactly 2 arguments";
//		return {};
//	}

//	if (!(context->argument(0).isValid() && !context->argument(0).isNull())) {
//		ADD_FAILURE() << "Assertion failure at\n"
//				<< QStringList(context->backtrace().mid(1)).join("\n").toStdString();
//		return {};
//	}

//	for (int i = 1; i < 3; ++i) {
//		if (!(context->argument(i).isValid() && context->argument(i).isString())) {
//			ADD_FAILURE() << "Assertion failure at\n"
//					<< QStringList(context->backtrace().mid(1)).join("\n").toStdString();
//			return {};
//		}
//	}

//	if (!(context->argument(3).isValid() && context->argument(3).isNumber())) {
//		ADD_FAILURE() << "Assertion failure at\n"
//				<< QStringList(context->backtrace().mid(1)).join("\n").toStdString();
//		return {};
//	}

//	QString dialogTitle = context->argument(1).toString();
//	QString fileName = context->argument(2).toString();
//	int mces = context->argument(3).toInt32();
//	MainWindow *mainWindow = qobject_cast<MainWindow *>(context->argument(0).toQObject());
//	QTimer::singleShot(mces, [=]() {
//		EXPECT_GT(mces, 0);
//		ASSERT_TRUE(mainWindow != nullptr);
//		QList<QDialog *> allDialogs = mainWindow->findChildren<QDialog *>();
//		ASSERT_FALSE(allDialogs.isEmpty());
//		for (QDialog * dialog : allDialogs) {
//			if (dialog->windowTitle() == dialogTitle || dialog->objectName() == dialogTitle) {
//				QFileDialog *fileDialog = dynamic_cast<QFileDialog *>(dialog);
//				ASSERT_TRUE(fileDialog != nullptr);
//				fileDialog
//				return;
//			}
//		}
//		FAIL() << "doesnt exist " << dialogTitle.toStdString() << " QFileDialog";
//	});

//	return {};
//}
//// mainwindow, имя диалога, имя файла, время
//QScriptValue scriptOpenFileInExpectedOpenDialog(QScriptContext *context, QScriptEngine *engine)
//{
//	Q_UNUSED(engine);

//	if (context->argumentCount() != 2) { // можно в этом месте делать -1 аргемент и развилку для топлвлдиалога
//		ADD_FAILURE() << "'scriptCloseExpectedOpenSaveDialog' shall have exactly 2 arguments";
//		return {};
//	}

//	if (!(context->argument(0).isValid() && !context->argument(0).isNull())) {
//		ADD_FAILURE() << "Assertion failure at\n"
//				<< QStringList(context->backtrace().mid(1)).join("\n").toStdString();
//		return {};
//	}

//	if (!(context->argument(1).isValid() && context->argument(1).isString())) {
//		ADD_FAILURE() << "Assertion failure at\n"
//				<< QStringList(context->backtrace().mid(1)).join("\n").toStdString();
//		return {};
//	}

//	if (!(context->argument(2).isValid() && context->argument(2).isNumber())) {
//		ADD_FAILURE() << "Assertion failure at\n"
//				<< QStringList(context->backtrace().mid(1)).join("\n").toStdString();
//		return {};
//	}

//	QString dialogTitle = context->argument(1).toString();
//	int mces = context->argument(2).toInt32();
//	MainWindow *mainWindow = qobject_cast<MainWindow *>(context->argument(0).toQObject());
//	QTimer::singleShot(mces, [=]() {
//		EXPECT_GT(mces, 0);
//		ASSERT_TRUE(mainWindow != nullptr);
//		QList<QDialog *> allDialogs = mainWindow->findChildren<QDialog *>();
//		ASSERT_FALSE(allDialogs.isEmpty());
//		for (QDialog * dialog : allDialogs) {
//			if (dialog->windowTitle() == dialogTitle || dialog->objectName() == dialogTitle) {
//				QFileDialog *fileDialog = dynamic_cast<QFileDialog *>(dialog);
//				ASSERT_TRUE(fileDialog != nullptr);
//				fileDialog->close();
//				return;
//			}
//		}
//		FAIL() << "doesnt exist " << dialogTitle.toStdString() << " QFileDialog";
//	});

//	return {};
//}
//QString fileName = utils::QRealFileDialog::getSaveFileName("SaveQRSProject", mMainWindow, dialogWindowTitle
//		, QFileInfo(mSaveFilePath).absoluteDir().absolutePath(), tr("QReal Save File(*.qrs)"));
// see ProjectManagerWrapper
