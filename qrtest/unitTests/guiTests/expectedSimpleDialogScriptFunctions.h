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

// может, стоит разбить на cpp + h?
// TODO::когда-то вынести в ScriptApi это (без ассертов...)
namespace qReal {
namespace gui {
namespace expected {
//// может, не вписывать имя диалога, а брать верхний?
// WARNING these methods doesnt work with openSaveFileDialogs
//	// считается, что buttons в окне без дублирования (по названию) (если не так, используйте другие методы)
//	// mainwindow, имя диалога, тип кнопки, название кнопки, время
//	// note имя и название кнопки могут быть objectname
//	QScriptValue scriptClickButtonInExpectedDialog(QScriptContext *context, QScriptEngine *engine);
//	// mainwindow, имя диалога, nameObject для lineEdit, текст для заполнения, время
//	// note название objectname, если пустая строка, ищется первый попавшийся lineedit
//	QScriptValue scriptFillLineEditInExpectedDialog(QScriptContext *context, QScriptEngine *engine);
//	// mainwindow, имя диалога, название(objectName) комбобокса, имя элемента, время
//	// note имя и название элемента могут быть objectname
//	// note мышь + клава
//	QScriptValue scriptChooseComboBoxItemInExpectedDialog(QScriptContext *context, QScriptEngine *engine);
}
}
}

using namespace qReal;
using namespace gui::expected;

void clickPushButton(QDialog *dialog, const QString &buttonText)
{
	const QList<QPushButton *> buttonList =
			dialog->findChildren<QPushButton *>(QString(), Qt::FindChildrenRecursively);
	for (QPushButton * button : buttonList) {
		if (button->text() == buttonText || button->objectName() == buttonText) {
			QTest::mouseClick(button, Qt::LeftButton);
			return;
		}
	}
	ADD_FAILURE() << "QPushButton (" << buttonText.toStdString() << ") was not found";

}

void clickRadioButton(QDialog *dialog, const QString &buttonText)
{
	const QList<QRadioButton *> buttonList =
			dialog->findChildren<QRadioButton *>(QString(), Qt::FindChildrenRecursively);
	for (QRadioButton * button : buttonList) {
		if (button->text() == buttonText || button->objectName() == buttonText) {
			QTest::mouseClick(button, Qt::LeftButton, Qt::NoModifier, QPoint(1, 1));
			ASSERT_TRUE(button->isChecked());
			return;
		}
	}
	ADD_FAILURE() << "QRadioButton (" << buttonText.toStdString() << ") was not found";
}

void clickCheckBox(QDialog *dialog, const QString &buttonText)
{
	const QList<QCheckBox *> buttonList =
			dialog->findChildren<QCheckBox *>(QString(), Qt::FindChildrenRecursively);
	for (QCheckBox * button : buttonList) {
		if (button->text() == buttonText || button->objectName() == buttonText) {
			bool isChecked = button->isChecked();
			QTest::mouseClick(button, Qt::LeftButton, Qt::NoModifier, QPoint(1, 1));
			ASSERT_TRUE(button->isChecked() != isChecked);
			return;
		}
	}
	ADD_FAILURE() << "QCheckBox (" << buttonText.toStdString() << ") was not found";
}

void clickButton(QDialog *dialog, const QString &buttonType, const QString &buttonText)
{
	enum {PUSHBUTTON = 1, RADIOBUTTON, CHECKBUTTON};
	std::map <std::string, int> mapping;

	mapping["QPushButton"] = PUSHBUTTON;
	mapping["QRadioButton"] = RADIOBUTTON;
	mapping["QCheckBox"] = CHECKBUTTON;

	switch (mapping[buttonType.toStdString()])
	{
		case PUSHBUTTON:
			clickPushButton(dialog, buttonText);
			break;
		case RADIOBUTTON:
			clickRadioButton(dialog, buttonText);
			break;
		case CHECKBUTTON:
			clickCheckBox(dialog, buttonText);
			break;
		default:
			ADD_FAILURE() << "'clickButton'\nneeded type: " << buttonType.toStdString() << " was not found";
	}
}

QScriptValue scriptClickButtonInExpectedDialog(QScriptContext *context, QScriptEngine *engine)
{
	Q_UNUSED(engine);

	if (context->argumentCount() != 5) {
		ADD_FAILURE() << "'scriptClickButtonInExpectedDialog' shall have exactly 5 arguments";
		return {};
	}

	if (!(context->argument(0).isValid() && !context->argument(0).isNull())) {
		ADD_FAILURE() << "Assertion failure at\n"
				<< QStringList(context->backtrace().mid(1)).join("\n").toStdString();
		return {};
	}

	for (int i = 1; i < 4; ++i) {
		if (!(context->argument(i).isValid() && context->argument(i).isString())) {
			ADD_FAILURE() << "Assertion failure at\n"
					<< QStringList(context->backtrace().mid(1)).join("\n").toStdString();
			return {};
		}
	}

	if (!(context->argument(4).isValid() && context->argument(4).isNumber())) {
		ADD_FAILURE() << "Assertion failure at\n"
				<< QStringList(context->backtrace().mid(1)).join("\n").toStdString();
		return {};
	}

	QString dialogTitle = context->argument(1).toString();
	QString buttonType = context->argument(2).toString();
	QString buttonText = context->argument(3).toString();
	int mces = context->argument(4).toInt32();
	MainWindow *mainWindow = qobject_cast<MainWindow *>(context->argument(0).toQObject());
	QTimer::singleShot(mces, [=]() {
		EXPECT_GT(mces, 0);
		ASSERT_TRUE(mainWindow != nullptr);
		QList<QDialog *> allDialogs = mainWindow->findChildren<QDialog *>();
		ASSERT_FALSE(allDialogs.isEmpty());
		for (QDialog * dialog : allDialogs) {
			if (dialog->windowTitle() == dialogTitle || dialog->objectName() == dialogTitle) {
				clickButton(dialog, buttonType, buttonText);
				return;
			}
		}
		FAIL() << "doesnt exist " << dialogTitle.toStdString() << " dialog";
	});

	return {};
}

void writeIn(QDialog *dialog, const QString &lineEditObjectName, const QString &text) {
	const QList<QLineEdit *> lineEditList = dialog->findChildren<QLineEdit *>(QString(), Qt::FindChildrenRecursively);
	for (QLineEdit * lineEdit : lineEditList) {
		if ((!lineEditObjectName.isEmpty() && lineEdit->objectName() == lineEditObjectName) || lineEditObjectName.isEmpty()) {
			QTest::mouseClick(lineEdit, Qt::LeftButton);
			QTest::keyClicks(lineEdit, text, Qt::NoModifier, 5);
			ASSERT_EQ(text, lineEdit->text());
			return;
		}
	}
	ADD_FAILURE() << "QLineEdit with " << lineEditObjectName.toStdString() << "objectName was not found";
}

// а логично ли? вообще можно создать метод, который общий, ему передавать функцию, что мы делаем с найденыым элементом.
//// only for special functions like as (ptr, str, str, str, int)
//bool checkContext(QScriptContext *context)
//{
//	if (context->argumentCount() != 5) {
//		ADD_FAILURE() << "'scriptFillLineEditInExpectedDialog' shall have exactly 5 arguments";
//		return false;
//	}

//	if (!(context->argument(0).isValid() && !context->argument(0).isNull())) {
//		ADD_FAILURE() << "Assertion failure at\n"
//				<< QStringList(context->backtrace().mid(1)).join("\n").toStdString();
//		return false;
//	}

//	for (int i = 1; i < 4; ++i) {
//		if (!(context->argument(i).isValid() && context->argument(i).isString())) {
//			ADD_FAILURE() << "Assertion failure at\n"
//					<< QStringList(context->backtrace().mid(1)).join("\n").toStdString();
//			return false;
//		}
//	}

//	if (!(context->argument(4).isValid() && context->argument(4).isNumber())) {
//		ADD_FAILURE() << "Assertion failure at\n"
//				<< QStringList(context->backtrace().mid(1)).join("\n").toStdString();
//		return false;
//	}
//}

QScriptValue scriptFillLineEditInExpectedDialog(QScriptContext *context, QScriptEngine *engine)
{
	Q_UNUSED(engine);

	if (context->argumentCount() != 5) {
		ADD_FAILURE() << "'scriptFillLineEditInExpectedDialog' shall have exactly 5 arguments";
		return {};
	}

	if (!(context->argument(0).isValid() && !context->argument(0).isNull())) {
		ADD_FAILURE() << "Assertion failure at\n"
				<< QStringList(context->backtrace().mid(1)).join("\n").toStdString();
		return {};
	}

	for (int i = 1; i < 4; ++i) {
		if (!(context->argument(i).isValid() && context->argument(i).isString())) {
			ADD_FAILURE() << "Assertion failure at\n"
					<< QStringList(context->backtrace().mid(1)).join("\n").toStdString();
			return {};
		}
	}

	if (!(context->argument(4).isValid() && context->argument(4).isNumber())) {
		ADD_FAILURE() << "Assertion failure at\n"
				<< QStringList(context->backtrace().mid(1)).join("\n").toStdString();
		return {};
	}

	QString dialogTitle = context->argument(1).toString();
	QString lineEditObjectName = context->argument(2).toString();
	QString text = context->argument(3).toString();
	int mces = context->argument(4).toInt32();
	MainWindow *mainWindow = qobject_cast<MainWindow *>(context->argument(0).toQObject());
	QTimer::singleShot(mces, [=]() {
		EXPECT_GT(mces, 0);
		ASSERT_TRUE(mainWindow != nullptr);
		QList<QDialog *> allDialogs = mainWindow->findChildren<QDialog *>();
		ASSERT_FALSE(allDialogs.isEmpty());
		for (QDialog * dialog : allDialogs) {
			if (dialog->windowTitle() == dialogTitle || dialog->objectName() == dialogTitle) {
				writeIn(dialog, lineEditObjectName, text);
				return;
			}
		}
		FAIL() << "doesnt exist " << dialogTitle.toStdString() << " dialog";
	});

	return {};
}

void pickNeededItem(QDialog *dialog, const QString &comboBoxObjectName, const QString &itemName)
{
	const QList<QComboBox *> boxList =
			dialog->findChildren<QComboBox *>(QString(), Qt::FindChildrenRecursively);
	for (QComboBox *box : boxList) {
		if (box->objectName() == comboBoxObjectName) {
			QTest::mouseClick(box, Qt::LeftButton);
			int currentIndex = box->currentIndex();
			ASSERT_TRUE(currentIndex != -1);
			int neededIndex = box->findText(itemName, Qt::MatchExactly | Qt::MatchCaseSensitive);
			ASSERT_TRUE(neededIndex != -1);
			Qt::Key	key = (currentIndex >= neededIndex) ? Qt::Key_Up : Qt::Key_Down;
			int direction = (key == Qt::Key_Up) ? 1 : -1;
			for (int i = currentIndex; i != neededIndex;) { // может попроще через while?
				QTest::keyClick(box, key);
				i -= direction;
			}
			QTest::keyClick(box, Qt::Key_Enter);
			return;
		}
	}
	ADD_FAILURE() << "QComboBox with " << comboBoxObjectName.toStdString() << "objectName was not found";
}

QScriptValue scriptChooseComboBoxItemInExpectedDialog(QScriptContext *context, QScriptEngine *engine)
{
	Q_UNUSED(engine);

	if (context->argumentCount() != 5) {
		ADD_FAILURE() << "'scriptChooseComboBoxItemInExpectedDialog' shall have exactly 5 arguments";
		return {};
	}

	if (!(context->argument(0).isValid() && !context->argument(0).isNull())) {
		ADD_FAILURE() << "Assertion failure at\n"
				<< QStringList(context->backtrace().mid(1)).join("\n").toStdString();
		return {};
	}

	for (int i = 1; i < 4; ++i) {
		if (!(context->argument(i).isValid() && context->argument(i).isString())) {
			ADD_FAILURE() << "Assertion failure at\n"
					<< QStringList(context->backtrace().mid(1)).join("\n").toStdString();
			return {};
		}
	}

	if (!(context->argument(4).isValid() && context->argument(4).isNumber())) {
		ADD_FAILURE() << "Assertion failure at\n"
				<< QStringList(context->backtrace().mid(1)).join("\n").toStdString();
		return {};
	}

	QString dialogTitle = context->argument(1).toString();
	QString comboBoxObjectName = context->argument(2).toString();
	QString itemName = context->argument(3).toString();
	int mces = context->argument(4).toInt32();
	MainWindow *mainWindow = qobject_cast<MainWindow *>(context->argument(0).toQObject());
	QTimer::singleShot(mces, [=]() {
		EXPECT_GT(mces, 0);
		ASSERT_TRUE(mainWindow != nullptr);
		QList<QDialog *> allDialogs = mainWindow->findChildren<QDialog *>();
		ASSERT_FALSE(allDialogs.isEmpty());
		for (QDialog * dialog : allDialogs) {
			if (dialog->windowTitle() == dialogTitle || dialog->objectName() == dialogTitle) {
				pickNeededItem(dialog, comboBoxObjectName, itemName);
				return;
			}
		}
		FAIL() << "doesnt exist " << dialogTitle.toStdString() << " dialog";
	});

	return {};
}

QScriptValue scriptClickButtonInExpectedTopLevelDialog(QScriptContext *context, QScriptEngine *engine)
{
	Q_UNUSED(engine);

	if (context->argumentCount() != 4) {
		ADD_FAILURE() << "'scriptClickButtonInExpectedTopLevelDialog' shall have exactly 4 arguments";
		return {};
	}

	if (!(context->argument(0).isValid() && !context->argument(0).isNull())) {
		ADD_FAILURE() << "Assertion failure at\n"
				<< QStringList(context->backtrace().mid(1)).join("\n").toStdString();
		return {};
	}

	for (int i = 1; i < 3; ++i) {
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

	QString buttonType = context->argument(1).toString();
	QString buttonText = context->argument(2).toString();
	int mces = context->argument(3).toInt32();
	MainWindow *mainWindow = qobject_cast<MainWindow *>(context->argument(0).toQObject());
	QTimer::singleShot(mces, [=]() {
		EXPECT_GT(mces, 0);
		ASSERT_TRUE(mainWindow != nullptr);
		QDialog *dialog = dynamic_cast <QDialog *> (QApplication::activeModalWidget());
		ASSERT_TRUE(dialog);
		clickButton(dialog, buttonType, buttonText);
	});

	return {};
}

// для отладки ---- ?
QScriptValue closeExpectedDialog(QScriptContext *context, QScriptEngine *engine)
{
Q_UNUSED(engine);
QString dialogTitle = context->argument(1).toString();
int mces = context->argument(2).toInt32();
MainWindow *mainWindow = qobject_cast<MainWindow *>(context->argument(0).toQObject());
QTimer::singleShot(mces, [=]() {
	QList<QDialog *> allDialogs = mainWindow->findChildren<QDialog *>();
	for (QDialog * dialog : allDialogs) {
		if (dialog->windowTitle() == dialogTitle || dialog->objectName() == dialogTitle) {
			dialog->close();
			qDebug() << "ЯТЕБЯ ЗАКРЫЛ";
			return;
		}
	}
});

return {};
}

