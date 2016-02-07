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

#include <QtTest/QTest>
#include <QtWidgets/QRadioButton>
#include <QtCore/QPropertyAnimation>
#include <QtGui/QWidgetList>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QCheckBox>

#include <qrkernel/exception/exception.h>
#include <qrutils/stringUtils.h>
#include <qrutils/inFile.h>
#include <editor/editorView.h>

#include "qrgui/mainWindow/mainWindow.h"
#include "qrgui/mainWindow/palette/draggableElement.h"

#include "qrgui/mainWindow/scriptAPI/scriptAPI.h"
#include "qrgui/mainWindow/scriptAPI/utils.h"
#include "qrgui/mainWindow/scriptAPI/guiFacade.h"
#include "qrgui/mainWindow/scriptAPI/virtualCursor.h"
#include "qrgui/mainWindow/scriptAPI/virtualKeyboard.h"
#include "qrgui/mainWindow/scriptAPI/hintAPI.h"
#include "qrgui/mainWindow/scriptAPI/sceneAPI.h"
#include "qrgui/mainWindow/scriptAPI/paletteAPI.h"

using namespace qReal;
using namespace gui;

Utils::Utils(ScriptAPI &scriptAPI, MainWindow &mainWindow, VirtualCursor &virtualCursor, HintAPI &hintAPI)
	: mScriptAPI(scriptAPI)
	, mMainWindow(mainWindow)
	, mVirtualCursor(virtualCursor)
	, mHintAPI(hintAPI)
{
}

void Utils::activateMenu(QMenu *menu) noexcept
{
	if (menu == nullptr) {
		throwScriptException(tr("Utils::activateMenu: (menu == nullptr). Given menu does not exist."));
		return;
	}

	QTest::keyClick(&mMainWindow, menu->title().at(1).toLatin1(), Qt::AltModifier);
}

void Utils::activateMenuAction(QMenu *menu, QAction *actionForExec) noexcept
{
	if (menu == nullptr) {
		throwScriptException(tr("Utils::activateMenuAction: (menu == nullptr). Given menu does not exist."));
		return;
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

	throwScriptException(tr("Action %1 (%2) does not exist in given menu")
			.arg(actionForExec->objectName())
			.arg(actionForExec->text()));
}

void Utils::fillLineEdit(const QString &widgetName, const QString &lineEditObjectName, const QString &text) noexcept
{
	doSmthInWidget(widgetName, lineEditObjectName, text, &Utils::writeIn);
}

void Utils::clickButton(const QString &widgetName, const QString &buttonType, const QString &buttonText) noexcept
{
	doSmthInWidget(widgetName, buttonType, buttonText, &Utils::clickNeededButton);
}

void Utils::chooseComboBoxItem(const QString &widgetName, const QString &comboBoxObjectName
		, const QString &itemName) noexcept
{
	doSmthInWidget(widgetName, comboBoxObjectName, itemName, &Utils::pickNeededItem);
}

void Utils::activateContextMenuAction(const QString &actionName) noexcept
{
	QMenu *contextMenu = dynamic_cast<QMenu *>(QApplication::activePopupWidget());
	if (contextMenu == nullptr) {
		throwScriptException(tr("Utils::activateContextMenuAction: contextMenu == nullptr. %1")
				.arg("Context menu does not exist"));
		return;
	}
	QTest::keyClick(contextMenu, Qt::Key_Down);

	const QList<QAction *> actions = contextMenu->actions();
	QAction *neededAction = nullptr;
	for (QAction * const action : actions) {
		if (action->objectName() == actionName || utils::StringUtils::deleteAmpersands(action->text()) == actionName) {
			neededAction = action;
		}
	}

	if (neededAction == nullptr) {
		throwScriptException(tr("Utils::activateContextMenuAction: neededAction == nullptr. Action (%1) %2")
			.arg(actionName)
			.arg("does not exist"));
		return;
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

void Utils::closeContextMenu() noexcept
{
	QMenu *contextMenu = dynamic_cast<QMenu *>(QApplication::activePopupWidget());
	if (!contextMenu) {
		throwScriptException(tr("Utils::closeContextMenu: contextMenu == nullptr. All context menues are closed."));
		return;
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
	const QList<DraggableElement *> paletteWidgets = mMainWindow.findChildren<DraggableElement *>();
	for (DraggableElement * const paletteElement : paletteWidgets) {
		qInfo() << paletteElement->id().toString();
	}
}

void Utils::writeIn(QWidget *widget, const QString &lineEditObjectName, const QString &text) const  noexcept
{
	const QList<QLineEdit *> lineEditList = widget->findChildren<QLineEdit *>(QString(), Qt::FindChildrenRecursively);
	for (QLineEdit * const lineEdit : lineEditList) {
		if (((!lineEditObjectName.isEmpty() && lineEdit->objectName() == lineEditObjectName)
				|| lineEditObjectName.isEmpty()) && lineEdit->isEnabled())
		{
			QTest::mouseClick(lineEdit, Qt::LeftButton);
			QTest::keyClicks(lineEdit, text, Qt::NoModifier, 20);
			if (text != lineEdit->text()) {
				throwScriptException(tr("Utils::writeIn: '%1' != '%2'. Inconsistency of texts for line edit in widget")
						.arg(text)
						.arg(lineEdit->text()));
			}

			return;
		}
	}

	throwScriptException(tr("QLineEdit with %1 objectName was not found").arg(lineEditObjectName));
}

void Utils::clickNeededButton(QWidget *widget, const QString &buttonType, const QString &buttonText) const noexcept
{
	enum class Button { PUSHBUTTON = 1, RADIOBUTTON, CHECKBUTTON };

	const QMap<QString, Button> map = { {"QPushButton", Button::PUSHBUTTON}
			, {"QRadioButton", Button::RADIOBUTTON}, {"QCheckBox",  Button::CHECKBUTTON}};

	if (!map.contains(buttonType)) {
		throwScriptException(tr("Utils::clickButton: requested type: '%1' was not found in widget").arg(buttonType));
		return;
	}

	switch (map.value(buttonType))
	{
		case Button::PUSHBUTTON:
			clickPushButton(widget, buttonText);
			break;
		case Button::RADIOBUTTON:
			clickRadioButton(widget, buttonText);
			break;
		case Button::CHECKBUTTON:
			clickCheckBox(widget, buttonText);
			break;
		default:
			// check enum, map, switch. we should not be here!
			qWarning() << "Utils::clickNeededButton: we should not be in default of a switch!";
	}
}

void Utils::pickNeededItem(QWidget *widget, const QString &comboBoxObjectName, const QString &itemName) const noexcept
{
	const QList<QComboBox *> boxList =
			widget->findChildren<QComboBox *>(QString(), Qt::FindChildrenRecursively);
	for (QComboBox *box : boxList) {
		if (box->objectName() == comboBoxObjectName && box->isEnabled() && box->isVisible()) {
			QTest::mouseClick(box, Qt::LeftButton);
			const int currentIndex = box->currentIndex();
			if (currentIndex == -1) {
				throwScriptException(tr("Utils::pickNeededItem: box->currentIndex() == -1. %1")
						.arg(" Current index has impossible value"));
				return;
			}

			const int neededIndex = box->findText(itemName, Qt::MatchExactly | Qt::MatchCaseSensitive);
			if (neededIndex == -1) {
				throwScriptException(tr("Utils::pickNeededItem: neededIndex == -1. %1")
						.arg(" Needed index has impossible value"));
				return;
			}

			const Qt::Key key = (currentIndex >= neededIndex) ? Qt::Key_Up : Qt::Key_Down;
			const int direction = (key == Qt::Key_Up) ? -1 : 1;
			for (int i = currentIndex; i != neededIndex; i += direction) {
				QTest::keyClick(box, key);
			}

			QTest::keyClick(box, Qt::Key_Enter);
			return;
		}
	}

	throwScriptException(tr("QComboBox with %1 objectName was not found in widget").arg(comboBoxObjectName));
}

void Utils::clickPushButton(QWidget *widget, const QString &buttonText) const noexcept
{
	const QList<QPushButton *> buttonList =
			widget->findChildren<QPushButton *>(QString(), Qt::FindChildrenRecursively);
	for (QPushButton * const button : buttonList) {
		if ((utils::StringUtils::deleteAmpersands(button->text()) == buttonText || button->objectName() == buttonText)
				&& button->isVisible() && button->isEnabled())
		{
			QTest::mouseClick(button, Qt::LeftButton);
			return;
		}
	}

	throwScriptException(tr("QPushButton (%1) was not found in widget").arg(buttonText));
}

void Utils::clickRadioButton(QWidget *widget, const QString &buttonText) const noexcept
{
	const QList<QRadioButton *> buttonList =
			widget->findChildren<QRadioButton *>(QString(), Qt::FindChildrenRecursively);
	for (QRadioButton * const button : buttonList) {
		if (((button->text() == buttonText || button->objectName() == buttonText))
				&& button->isVisible() && button->isEnabled())
		{
			QTest::mouseClick(button, Qt::LeftButton, Qt::NoModifier, QPoint(1, 1));
			if (!button->isChecked()) {
				throwScriptException(tr("Utils::clickRadioButton: !button->isChecked(). %1")
						.arg("Clicking on the radio button in widget is failed."));
			}

			return;
		}
	}

	throwScriptException(tr("QRadioButton (%1) was not found in widget").arg(buttonText));
}

void Utils::clickCheckBox(QWidget *widget, const QString &buttonText) const noexcept
{
	const QList<QCheckBox *> buttonList =
			widget->findChildren<QCheckBox *>(QString(), Qt::FindChildrenRecursively);
	for (QCheckBox * button : buttonList) {
		if ((utils::StringUtils::deleteAmpersands(button->text()) == buttonText || button->objectName() == buttonText)
				&& button->isVisible() && button->isEnabled())
		{
			const bool isChecked = button->isChecked();
			QTest::mouseClick(button, Qt::LeftButton, Qt::NoModifier, QPoint(1, 1));
			if (button->isChecked() == isChecked) {
				throwScriptException(tr("Utils::clickCheckBox: button->isChecked() == isChecked. %1")
						.arg("The value of the check box in widget was not changed."));
			}

			return;
		}
	}

	throwScriptException(tr("QCheckBox (%1) was not found in widget").arg(buttonText));
}

void Utils::throwScriptException(const QString &msg) const noexcept
{
	mScriptAPI.engine()->currentContext()->throwError(msg);
}

void Utils::doSmthInWidget(const QString &widgetName, const QString &identifier
		, const QString &associatedAction
		, std::function<void(const Utils &, QWidget *, const QString &, const QString &)> doAction) const noexcept
{
	const QList<QWidget *> allWidgets = mMainWindow.findChildren<QWidget *>();
	for (QWidget * const widget : allWidgets) {
		if (widget->windowTitle() == widgetName || widget->objectName() == widgetName) {
			doAction(*this, widget, identifier, associatedAction);
			return;
		}
	}

	throwScriptException(tr("Dialog (%1) does not exist").arg(widgetName));
}
