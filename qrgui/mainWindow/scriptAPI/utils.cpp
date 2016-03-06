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

static const int buttonFrameThickness = 1;

#include <QtTest/QTest>
#include <QtWidgets/QRadioButton>
#include <QtCore/QPropertyAnimation>
#include <QtGui/QWidgetList>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QMenuBar>

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

Utils::Utils(ScriptAPI &scriptAPI, MainWindow &mainWindow, VirtualCursor &virtualCursor
	   , VirtualKeyboard &virtualKeyboard, HintAPI &hintAPI)
	: mScriptAPI(scriptAPI)
	, mMainWindow(mainWindow)
	, mVirtualCursor(virtualCursor)
	, mVirtualKeyboard(virtualKeyboard)
	, mHintAPI(hintAPI)
{
}

void Utils::activateMenu(QMenu *menu) noexcept
{
	if (menu == nullptr) {
		throwScriptException(tr("Utils::activateMenu: (menu == nullptr). Given menu does not exist."));
		return;
	}

	QMenuBar *menubar = dynamic_cast<QMenuBar *>(menu->parentWidget());
	if (menubar == nullptr) {
		throwScriptException(tr("Utils::activateMenu: (menubar == nullptr). Menubar does not exist."));
		return;
	}

	const QPoint centerPoint = menubar->actionGeometry(menu->menuAction()).center();
	QTest::mouseClick(menubar, Qt::LeftButton, Qt::NoModifier
			, centerPoint);
	mVirtualCursor.moveToXY(centerPoint.x(), centerPoint.y(), 50);
	QTest::qWait(50);
	if (!menu->activeAction()) {
		QTest::keyClick(menu, Qt::Key_Down);
	}
}

void Utils::activateMenuAction(QMenu *menu, QAction *actionForExec) noexcept
{
	if (menu == nullptr) {
		throwScriptException(tr("Utils::activateMenuAction: (menu == nullptr). Given menu does not exist."));
		return;
	}

	if (menu->isHidden()) {
		throwScriptException(tr("Utils::activateMenuAction: (menu is hidden). Given menu is not open."));
		return;
	}

	if (!menu->activeAction()) {
		throwScriptException(tr("Utils::activateMenuAction: (actions are not selected). "
				"Given menu without selected actions."));
		return;
	}

	for (QAction * const action : menu->actions()) {
		if (action == actionForExec) {
			const bool isSubmenu = menu->children().contains(actionForExec->menu());
			const Qt::Key key = isSubmenu ? Qt::Key_Right : Qt::Key_Enter;
			const QPoint centerPoint = menu->actionGeometry(actionForExec).center();
			mVirtualCursor.moveToXY(centerPoint.x(), centerPoint.y(), 50);
			QTest::qWait(50);
			QTest::mouseClick(menu, Qt::LeftButton, Qt::NoModifier, centerPoint);
			if (isSubmenu) {
				QTest::keyClick(menu, key);
			}

			return;
		}

		if (!action->isSeparator()) {
			QTest::keyClick(menu, Qt::Key_Down);
			QTest::qWait(50);
		}
	}

	throwScriptException(tr("Action %1 (%2) does not exist in given menu")
			.arg(actionForExec->objectName(), actionForExec->text()));
}

void Utils::fillInputWidget(const QString &widgetName, const QString &lineEditObjectName, const QString &text) noexcept
{
	doSmthInWidget(widgetName, lineEditObjectName, text, &Utils::writeInto);
}

void Utils::clickButton(const QString &widgetName, const QString &buttonText) noexcept
{
	doSmthInWidget(widgetName, QString(), buttonText, &Utils::clickAbstractButton);
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
		throwScriptException(tr("Utils::activateContextMenuAction: contextMenu == nullptr. "
				"Context menu does not exist"));
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
		throwScriptException(tr("Utils::activateContextMenuAction: neededAction == nullptr. Action (%1) "
				"does not exist").arg(actionName));
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

void Utils::writeInto(QWidget *widget, const QString &editObjectName, const QString &text) const noexcept
{
	QList<QWidget *> editList = widget->findChildren<QWidget *>(QString(), Qt::FindChildrenRecursively);
	for (QWidget * const widget : editList) {
		if (widget->testAttribute(Qt::WA_InputMethodEnabled)
				&& widget->isEnabled()
				&& ((!editObjectName.isEmpty() && widget->objectName() == editObjectName) || editObjectName.isEmpty()))
		{
			QTest::mouseClick(widget, Qt::LeftButton);
			QTest::keyClicks(widget, text, Qt::NoModifier, 20);

			QLineEdit *lineEdit = dynamic_cast<QLineEdit *>(widget); // we can add other types for throwScript
			if (lineEdit && text != lineEdit->text()) {
				throwScriptException(tr("Utils::writeInto: '%1' != '%2'. Inconsistency of texts in widget")
						.arg(text, lineEdit->text()));
			}

			return;
		}
	}

	throwScriptException(tr("Input widget with %1 objectName was not found").arg(editObjectName));
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
				throwScriptException(tr("Utils::pickNeededItem: box->currentIndex() == -1. "
						"Current index has impossible value"));
				return;
			}

			const int neededIndex = box->findText(itemName, Qt::MatchExactly | Qt::MatchCaseSensitive);
			if (neededIndex == -1) {
				throwScriptException(tr("Utils::pickNeededItem: neededIndex == -1. "
						"Needed index has impossible value"));
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

void Utils::clickAbstractButton(QWidget *widget, const QString &buttonType, const QString &buttonText) const noexcept
{
	Q_UNUSED(buttonType)
	const QList<QAbstractButton *> buttonList =
			widget->findChildren<QAbstractButton *>(QString(), Qt::FindChildrenRecursively);
	for (QAbstractButton * const button : buttonList) {
		if ((utils::StringUtils::deleteAmpersands(button->text()) == buttonText || button->objectName() == buttonText)
				&& button->isVisible() && button->isEnabled())
		{
			const bool isChecked = button->isChecked();

			QTest::mouseClick(button, Qt::LeftButton, Qt::NoModifier, QPoint(buttonFrameThickness
					, buttonFrameThickness));
			if (!button->isChecked() && dynamic_cast<QRadioButton *>(button)) {
				throwScriptException(tr("Utils::clickAbstractButton: !button->isChecked(). "
						"Clicking on the radiobutton in widget is failed."));
			}

			if (dynamic_cast<QCheckBox *>(button) && isChecked == button->isChecked()) {
				throwScriptException(tr("Utils::clickAbstractButton: button->isChecked() == isChecked. "
						"The value of the check box in widget was not changed."));
			}

			return;
		}
	}

	throwScriptException(tr("Button (%1) was not found in widget").arg(buttonText));
}

void Utils::throwScriptException(const QString &msg) const noexcept
{
	mScriptAPI.engine()->currentContext()->throwError(msg);
}

void Utils::doSmthInWidget(const QString &widgetName, const QString &identifier, const QString &associatedAction
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
