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

#pragma once

#include <QtWidgets/QMenu>
#include <QtWidgets/QAction>
#include <functional>

namespace qReal {

class MainWindow;

namespace gui {

class GuiFacade;
class VirtualCursor;
class VirtualKeyboard;
class SceneAPI;
class PaletteAPI;
class HintAPI;
class ScriptAPI;

/// This class provides scripting access to some complicated actions
/// and usefull methods for test writing, independent of UI, scene, palette etc.
class Utils : public QObject
{
	Q_OBJECT

public:
	Utils(ScriptAPI &scriptAPI, MainWindow &mainWindow, VirtualCursor &virtualCursor
			, VirtualKeyboard &virtualKeyboard, HintAPI &hintAPI);

	// ---------- Actions: ----------//
	// Don't forget about virtual cursor's moves.

	/// Activates menu getting with pointer.
	/// @note This method works with a mouse.
	Q_INVOKABLE void activateMenu(QMenu *menu) noexcept;

	/// Activates \a actionForExec in corresponding \a menu.
	/// @warning Use this method only after opening of the assigned menu.
	/// This method implementation emits necessary signal aboutToShow() (see the bug description).
	/// In the normal case aboutToShow() should be emitted independently singly.
	/// @see The appropriative bug exists: https://bugs.launchpad.net/appmenu-qt5/+bug/1449373.
	/// @note This method interacts with QMenu through the virtual keyboard, not virtual mouse.
	/// And the delay between key clicks equals ~50 msec.
	/// @todo: realise visible mouse moves with hints or virtual cursor.
	// todo: specify waiting time (delay) for all methods.
	Q_INVOKABLE void activateMenuAction(QMenu *menu, QAction *actionForExec) noexcept;

	/// Types \a text in a text field \a lineEditObjectName inside a widget \a widgetName.
	/// @todo: realise visible mouse moves with hints or virtual cursor.
	Q_INVOKABLE void fillInputWidget(const QString &widgetName, const QString &lineEditObjectName
			, const QString &text) noexcept;

	/// Clicks a button having \a buttonText printed on the button inside a widget with name \a widgetName.
	/// @todo: realise visible mouse moves with hints or virtual cursor.
	Q_INVOKABLE void clickButton(const QString &widgetName, const QString &buttonText) noexcept;

	/// Clicks a certain item in a certain comboBox placed inside a certain widget.
	/// @todo: realise visible mouse moves with hints or virtual cursor.
	Q_INVOKABLE void chooseComboBoxItem(const QString &widgetName
			, const QString &comboBoxObjectName, const QString &itemName) noexcept;

	/// By name \a actionName activates some action in a context menu.
	/// @todo: realise visible mouse moves with hints or virtual cursor.
	Q_INVOKABLE void activateContextMenuAction(const QString &actionName) noexcept;

	/// Closes top level context menu.
	/// @todo: realise visible mouse moves with hints or virtual cursor.
	Q_INVOKABLE void closeContextMenu() noexcept;

	// -------- Getters: ---------//

	/// @returns the geometry of \a action in \a menu.
	Q_INVOKABLE QRect actionRect(const QMenu *menu, QAction *action) const;

	/// @returns the position of \a widget.
	Q_INVOKABLE QPoint topLeftWidgetCorner(const QWidget *widget) const;

	/// @returns the number of occurrences of value in the list.
	Q_INVOKABLE int length(const QStringList &list) const;

	// ----------- Validators: ------------ //

	/// @returns true if \a action is enabled and visible, otherway returns false.
	Q_INVOKABLE bool isEnabledAndVisible(const QAction *action) const;

	/// @returns true if \a widget is enabled and visible, otherway returns false.
	Q_INVOKABLE bool isEnabledAndVisible(const QWidget *widget) const;

	/// @returns true if \a action is checked, otherway returns false.
	Q_INVOKABLE bool actionIsChecked(const QAction *action) const;

	/// @returns true if \a action is a checkable action, otherway returns false.
	Q_INVOKABLE bool actionIsCheckable(const QAction *action) const;

	// ----------- Debugging: ------------ //

	/// Prints usefull information about palette elements.
	/// @note Usefull method for debugging and test scripts writing.
	Q_INVOKABLE void printPaletteElementsInfo() const;

private:
	void writeInto(QWidget *widget, const QString &editObjectName, const QString &text) const noexcept;
	void pickNeededItem(QWidget *widget, const QString &comboBoxObjectName, const QString &itemName) const noexcept;
	void clickAbstractButton(QWidget *widget, const QString &buttonType, const QString &buttonText) const noexcept;
	void throwScriptException(const QString &msg) const noexcept;
	void doSmthInWidget(const QString &widgetName, const QString &identifier
			, const QString &buttonText
			, std::function<void(const Utils &, QWidget *
			, const QString &, const QString &)>doAction) const noexcept;

	ScriptAPI &mScriptAPI;
	MainWindow &mMainWindow;
	VirtualCursor &mVirtualCursor;
	VirtualKeyboard &mVirtualKeyboard;
	HintAPI &mHintAPI;
};

}
}
