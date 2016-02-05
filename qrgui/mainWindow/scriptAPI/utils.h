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
/// and usefull methods for test writing, independent of ui, scene, palette etc.
class Utils : public QObject
{
	Q_OBJECT

public:
	Utils(ScriptAPI &scriptAPI, MainWindow &mainWindow, VirtualCursor &virtualCursor, HintAPI &hintAPI);

	// Actions: Don't forget about virtual cursor's moves.
	/// @note This method works with a keyboard (not a mouse).
	/// Activates menu getting with pointer.
	// todo: realise visible mouse moves with hints or virtual cursor.
	Q_INVOKABLE void activateMenu(QMenu *menu) noexcept;

	/// @warning Use this method only after opening of the assigned menu.
	/// @note This method works with a keyboard (not a mouse).
	/// Activates \a actionForExec in corresponding \a menu.
	// todo: realise visible mouse moves with hints or virtual cursor.
	Q_INVOKABLE void activateMenuAction(QMenu *menu, QAction *actionForExec) noexcept;

	// todo: realise visible mouse moves with hints or virtual cursor.
	/// Types \a text in a text field \a lineEditObjectName inside a widget \a widgetName.
	Q_INVOKABLE void fillLineEdit(const QString &widgetName, const QString &lineEditObjectName
			, const QString &text) noexcept;

	// todo: realise visible mouse moves with hints or virtual cursor.
	/// Clicks a button having \a buttonType with \a buttonText printed on the button inside corresponding
	/// widget with name \a widgetName.
	Q_INVOKABLE void clickButton(const QString &widgetName, const QString &buttonType
			, const QString &buttonText) noexcept;

	// todo: realise visible mouse moves with hints or virtual cursor.
	/// Clicks a certain item in a certain comboBox placed inside a certain widget.
	Q_INVOKABLE void chooseComboBoxItem(const QString &widgetName
			, const QString &comboBoxObjectName, const QString &itemName) noexcept;

	// todo: realise visible mouse moves with hints or virtual cursor.
	/// By name \a actionName activates some action in a context menu.
	Q_INVOKABLE void activateContextMenuAction(const QString &actionName) noexcept;

	// todo: realise visible mouse moves with hints or virtual cursor.
	/// Closes top level context menu.
	Q_INVOKABLE void closeContextMenu() noexcept;

	// Returners:
	/// @returns the geometry of \a action in \a menu.
	Q_INVOKABLE QRect actionRect(const QMenu *menu, QAction *action) const;

	/// @returns the position of \a widget.
	Q_INVOKABLE QPoint topLeftWidgetCorner(const QWidget *widget) const;

	/// @returns the number of occurrences of value in the list.
	Q_INVOKABLE int length(const QStringList &list) const;

	// Checkers:
	/// @returns true if \a action is enabled and visible, otherway returns false.
	Q_INVOKABLE bool isEnabledAndVisible(const QAction *action) const;

	/// @returns true if \a widget is enabled and visible, otherway returns false.
	Q_INVOKABLE bool isEnabledAndVisible(const QWidget *widget) const;

	/// @returns true if \a action is checked, otherway returns false.
	Q_INVOKABLE bool actionIsChecked(const QAction *action) const;

	/// @returns true if \a action is a checkable action, otherway returns false.
	Q_INVOKABLE bool actionIsCheckable(const QAction *action) const;

	// Informators:
	/// @note usefull method for debugging and test scripts writing.
	/// Prints usefull information about palette elements.
	Q_INVOKABLE void printPaletteElementsInfo() const;

private:
	void writeIn(QWidget *widget, const QString &lineEditObjectName, const QString &text) const noexcept;
	void clickNeededButton(QWidget *widget, const QString &buttonType, const QString &buttonText) const noexcept;
	void pickNeededItem(QWidget *widget, const QString &comboBoxObjectName, const QString &itemName) const noexcept;
	void clickPushButton(QWidget *widget, const QString &buttonText) const noexcept;
	void clickRadioButton(QWidget *widget, const QString &buttonText) const noexcept;
	void clickCheckBox(QWidget *widget, const QString &buttonText) const noexcept;
	void throwScriptException(const QString &msg) const noexcept;
	void doSmthInWidget(const QString &widgetName, const QString &identifier
			, const QString &buttonText
			, std::function<void(const Utils &, QWidget *
			, const QString &, const QString &)>doAction) const noexcept;

	ScriptAPI &mScriptAPI;
	MainWindow &mMainWindow;
	VirtualCursor &mVirtualCursor;
	HintAPI &mHintAPI;
};

}
}
