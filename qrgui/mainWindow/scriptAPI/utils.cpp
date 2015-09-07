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

#include "Utils.h"

#include <QTest>
#include <QDebug>
#include <QRadioButton>

#include <QtCore/QPropertyAnimation>
#include <QtGui/QWidgetList>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>

#include <qrkernel/exception/exception.h>
#include <qrutils/inFile.h>
#include <editor/editorView.h>
#include <models/commands/createElementCommand.h>

#include "qrgui/mainWindow/mainWindow.h"
#include "./qrgui/mainWindow/palette/draggableElement.h"

#include "./qrgui/mainWindow/scriptAPI/scriptAPI.h"
#include "./qrgui/mainWindow/scriptAPI/utils.h"
#include "./qrgui/mainWindow/scriptAPI/guiFacade.h"
#include "./qrgui/mainWindow/scriptAPI/virtualCursor.h"
#include "./qrgui/mainWindow/scriptAPI/virtualKeyboard.h"
#include "./qrgui/mainWindow/scriptAPI/hintAPI.h"
#include "./qrgui/mainWindow/scriptAPI/sceneAPI.h"
#include "./qrgui/mainWindow/scriptAPI/paletteAPI.h"
#include "./qrgui/mainWindow/scriptAPI/scriptRegisterMetaTypes.h"


using namespace qReal;
using namespace gui;

Utils::Utils(ScriptAPI &scriptAPI, MainWindow &mainWindow, VirtualCursor &virtualCursor, HintAPI &hintAPI)
	: mScriptAPI(scriptAPI)
	, mMainWindow(mainWindow)
	, mVirtualCursor(virtualCursor)
	, mHintAPI(hintAPI)
{
}

void Utils::activateMenu(QMenu *menu)
{
	if (menu == nullptr) {
		throwScriptException("Utils::activateMenu: (menu == nullptr)");
	}
	QTest::keyClick(&mMainWindow, menu->title().at(1).toLatin1(), Qt::AltModifier);
}

void Utils::activateMenuAction(QMenu *menu, QAction *actionForExec)
{
	if (menu == nullptr) {
		throwScriptException("Utils::activateMenuAction: (menu == nullptr)");
	}
	for (const QAction *action : menu->actions()) {
		if (action == actionForExec) {
			QTest::keyClick(menu, Qt::Key_Enter);
			return;
		}
		if (!action->isSeparator()) {
			QTest::qWait(25);
			QTest::keyClick(menu, Qt::Key_Down);
			QTest::qWait(25);
		}
	}
	throwScriptException("doesnt exist " + actionForExec->objectName() + " " + actionForExec->text() + " action");
}

void Utils::fillLineEdit(const QString &widgetName, const QString &lineEditObjectName, const QString &text)
{
	doSmthInWidget(widgetName, lineEditObjectName, text, (&writeIn));
//	QList<QWidget *> allWidgets = mMainWindow.findChildren<QWidget *>();
//	for (QWidget * widget : allWidgets) {
//		if (widget->windowTitle() == widgetName || widget->objectName() == widgetName) {
//			writeIn(widget, lineEditObjectName, text);
//			return;
//		}
//	}
//	throwScriptException("doesnt exist " + widgetName + " dialog");
}

void Utils::clickButton(const QString &widgetName, const QString &buttonType, const QString &buttonText)
{
	doSmthInWidget(widgetName, buttonType, buttonText, &clickNeededButton);
//	QList<QWidget *> allWidgets = mMainWindow.findChildren<QWidget *>();
//	for (QWidget * widget : allWidgets) {
//		if (widget->windowTitle() == widgetName || widget->objectName() == widgetName) {
//			clickNeededButton(widget, buttonType, buttonText);
//			return;
//		}
//	}
//	throwScriptException("doesnt exist " + widgetName + " dialog");
}

void Utils::chooseComboBoxItem(const QString &widgetName, const QString &comboBoxObjectName, const QString &itemName)
{
	doSmthInWidget(widgetName, comboBoxObjectName, itemName, &pickNeededItem);
//	QList<QWidget *> allWidgets = mMainWindow.findChildren<QWidget *>();
//	for (QWidget * widget : allWidgets) {
//		if (widget->windowTitle() == widgetName || widget->objectName() == widgetName) {
//			pickNeededItem(widget, comboBoxObjectName, itemName);
//			return;
//		}
//	}
//	throwScriptException("doesnt exist " + widgetName + " dialog");
}

void Utils::activateContextMenuAction(const QString &actionName)
{
	QMenu *contextMenu = dynamic_cast<QMenu *>(QApplication::activePopupWidget());
	QTest::keyClick(contextMenu, Qt::Key_Down);
	if (contextMenu == nullptr) {
		throwScriptException("Utils::activateContextMenuAction: contextMenu == nullptr");
	}
	QList<QAction *> actions = contextMenu->actions();
	QAction *neededAction = nullptr;
	for (QAction *action: actions) {
		if (action->objectName() == actionName || action->text() == actionName) {
			neededAction = action;
		}
	}
	if (neededAction == nullptr) {
		throwScriptException("Utils::activateContextMenuAction: neededAction == nullptr");
	}
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
}

void Utils::closeContextMenu()
{
	QMenu *contextMenu = dynamic_cast<QMenu *>(QApplication::activePopupWidget());
	if (!contextMenu) {
		throwScriptException("Utils::closeContextMenu: contextMenu == nullptr");
	}
	QTest::keyClick(contextMenu, Qt::Key_Escape);
}

QRect Utils::actionRect(const QMenu *menu, QAction *action) const
{
	return menu->actionGeometry(action);
}

QPoint Utils::topLeftWidgetCorner(const QWidget *widget) const
{
	return widget->pos();
}

int Utils::length(const QStringList &list) const
{
	return list.length();
}

bool Utils::isEnabledAndVisible(const QAction *action) const
{
	return action->isEnabled() && action->isVisible();
}

bool Utils::isEnabledAndVisible(const QWidget *widget) const
{
	return widget->isEnabled() && widget->isVisible();
}

bool Utils::actionIsChecked(const QAction *action) const
{
	return action->isChecked();
}

bool Utils::actionIsCheckable(const QAction *action) const
{
	return action->isCheckable();
}

void Utils::printPaletteElementsInfo() const
{
	QList<DraggableElement *> const paletteWidgets = mMainWindow.findChildren<DraggableElement *>();
	for (DraggableElement * const paletteElement : paletteWidgets) {
		paletteElement->id().toString();
	}
}

void Utils::writeIn(QWidget *widget, const QString &lineEditObjectName, const QString &text)
{
	const QList<QLineEdit *> lineEditList = widget->findChildren<QLineEdit *>(QString(), Qt::FindChildrenRecursively);
	for (QLineEdit * lineEdit : lineEditList) {
		if (((!lineEditObjectName.isEmpty() && lineEdit->objectName() == lineEditObjectName)
				|| lineEditObjectName.isEmpty()) && lineEdit->isEnabled()) {
			QTest::mouseClick(lineEdit, Qt::LeftButton);
			QTest::keyClicks(lineEdit, text, Qt::NoModifier, 20);
			if (text != lineEdit->text()) {
				throwScriptException("Utils::writeIn: '" + text + "' != '" + lineEdit->text() + "'");
			}
			return;
		}
	}
	throwScriptException("QLineEdit with " + lineEditObjectName + " objectName was not found");
}

void Utils::clickNeededButton(QWidget *widget, const QString &buttonType, const QString &buttonText)
{
	enum {PUSHBUTTON = 1, RADIOBUTTON, CHECKBUTTON};
	std::map <std::string, int> mapping;

	mapping["QPushButton"] = PUSHBUTTON;
	mapping["QRadioButton"] = RADIOBUTTON;
	mapping["QCheckBox"] = CHECKBUTTON;

	switch (mapping[buttonType.toStdString()])
	{
		case PUSHBUTTON:
			clickPushButton(widget, buttonText);
			break;
		case RADIOBUTTON:
			clickRadioButton(widget, buttonText);
			break;
		case CHECKBUTTON:
			clickCheckBox(widget, buttonText);
			break;
		default:
			throwScriptException("Utils::clickButton: needed type: '" + buttonType + "' was not found");
	}

//	const QList<QAbstractButton*> buttonList =
//			widget->findChildren<QAbstractButton *>(QString(), Qt::FindChildrenRecursively);
//	const char *typeName = buttonType.toLocal8Bit().data();
//	for (QAbstractButton * button : buttonList) {
//		if ((button->text() == buttonText || button->objectName() == buttonText)
//				&& button->inherits(typeName) && button->isVisible() && button->isEnabled()) {
//			QTest::mouseClick(button, Qt::LeftButton, Qt::NoModifier, QPoint(1, 1));
//			return;
//		}
//	}
}

void Utils::pickNeededItem(QWidget *widget, const QString &comboBoxObjectName, const QString &itemName)
{
	const QList<QComboBox *> boxList =
			widget->findChildren<QComboBox *>(QString(), Qt::FindChildrenRecursively);
	for (QComboBox *box : boxList) {
		if (box->objectName() == comboBoxObjectName && box->isEnabled() && box->isVisible()) {
			QTest::mouseClick(box, Qt::LeftButton);
			int currentIndex = box->currentIndex();
			if (currentIndex == -1) {
				throwScriptException(QString("Utils::pickNeededItem: ") + "box->currentIndex() == -1");
			}
			int neededIndex = box->findText(itemName, Qt::MatchExactly | Qt::MatchCaseSensitive);
			if (neededIndex == -1) {
				throwScriptException(QString("Utils::pickNeededItem: ") + "neededIndex == -1");
			}
			Qt::Key	key = (currentIndex >= neededIndex) ? Qt::Key_Up : Qt::Key_Down;
			int direction = (key == Qt::Key_Up) ? 1 : -1;
			for (int i = currentIndex; i != neededIndex;) {
				QTest::keyClick(box, key);
				i -= direction;
			}
			QTest::keyClick(box, Qt::Key_Enter);
			return;
		}
	}
	throwScriptException("QComboBox with " + comboBoxObjectName + "objectName was not found");
}

void Utils::clickPushButton(QWidget *widget, const QString &buttonText)
{
	const QList<QPushButton *> buttonList =
			widget->findChildren<QPushButton *>(QString(), Qt::FindChildrenRecursively);
	for (QPushButton * button : buttonList) {
		if ((button->text() == buttonText || button->objectName() == buttonText)
				&& button->isVisible() && button->isEnabled()) {
			QTest::mouseClick(button, Qt::LeftButton);
			return;
		}
	}
	throwScriptException("QPushButton (" + buttonText + ") was not found");
}

void Utils::clickRadioButton(QWidget *widget, const QString &buttonText)
{
	const QList<QRadioButton *> buttonList =
			widget->findChildren<QRadioButton *>(QString(), Qt::FindChildrenRecursively);
	for (QRadioButton * button : buttonList) {
		if (((button->text() == buttonText || button->objectName() == buttonText))
			&& button->isVisible() && button->isEnabled()) {
			QTest::mouseClick(button, Qt::LeftButton, Qt::NoModifier, QPoint(1, 1));
			if (!button->isChecked()) {
				throwScriptException("Utils::clickRadioButton: !button->isChecked()");
			}
			return;
		}
	}
	throwScriptException("QRadioButton (" + buttonText + ") was not found");
}

void Utils::clickCheckBox(QWidget *widget, const QString &buttonText)
{
	const QList<QCheckBox *> buttonList =
			widget->findChildren<QCheckBox *>(QString(), Qt::FindChildrenRecursively);
	for (QCheckBox * button : buttonList) {
		if ((button->text() == buttonText || button->objectName() == buttonText)
				&& button->isVisible() && button->isEnabled()) {
			bool isChecked = button->isChecked();
			QTest::mouseClick(button, Qt::LeftButton, Qt::NoModifier, QPoint(1, 1));
			if (button->isChecked() == isChecked) {
					throwScriptException("Utils::clickCheckBox: button->isChecked() == isChecked");
			}
			return;
		}
	}
	throwScriptException("QCheckBox (" + buttonText + ") was not found");
}

void Utils::throwScriptException(const QString &msg)
{
	mScriptAPI.getEngine()->currentContext()->throwError(msg);
}

void Utils::doSmthInWidget(const QString &widgetName, const QString &identifier
					, const QString &associatedAction, void (qReal::gui::Utils::*actionF)(QWidget*, const QString&, const QString &))
{
	QList<QWidget *> allWidgets = mMainWindow.findChildren<QWidget *>();
	for (QWidget * widget : allWidgets) {
		if (widget->windowTitle() == widgetName || widget->objectName() == widgetName) {
			(this->*actionF)(widget, identifier, associatedAction);
			return;
		}
	}
	throwScriptException("doesnt exist " + widgetName + " dialog");
}
