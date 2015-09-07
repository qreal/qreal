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

#include <QMenu>
#include <QAction>

class QComboBox;
class QAbstractScrollArea;

namespace qReal {

class MainWindow;

namespace gui {

class DraggableElement;

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
	explicit Utils(ScriptAPI &scriptAPI, MainWindow &mainWindow, VirtualCursor &virtualCursor, HintAPI &hintAPI);

	// Actions: dont forget about virtual cursor's moves
	/// @note This method works with a keyboard (not a mouse)
	/// @todo: realise visible mouse moves with hints or virtual cursor
	Q_INVOKABLE void activateMenu(QMenu *menu);

	/// @warning Use this method only after opening of the assigned menu
	/// @note This method works with a keyboard (not a mouse)
	/// @todo: realise visible mouse moves with hints or virtual cursor
	Q_INVOKABLE void activateMenuAction(QMenu *menu, QAction *actionForExec);

	/// @todo: realise visible mouse moves with hints or virtual cursor
	Q_INVOKABLE void fillLineEdit(const QString &widgetName, const QString &lineEditObjectName, const QString &text);

	/// @todo: realise visible mouse moves with hints or virtual cursor
	Q_INVOKABLE void clickButton(const QString &widgetName, const QString &buttonType, const QString &buttonText);

	/// @todo: realise visible mouse moves with hints or virtual cursor
	Q_INVOKABLE void chooseComboBoxItem(const QString &widgetName, const QString &comboBoxObjectName, const QString &itemName);

	/// @todo: realise visible mouse moves with hints or virtual cursor
	Q_INVOKABLE void activateContextMenuAction(const QString &actionName);

	/// @todo: realise visible mouse moves with hints or virtual cursor
	Q_INVOKABLE void closeContextMenu();

	// Returners:
	Q_INVOKABLE QRect actionRect(const QMenu *menu, QAction *action) const;

	Q_INVOKABLE QPoint topLeftWidgetCorner(const QWidget *widget) const;

	Q_INVOKABLE int length(const QStringList &list) const;

	// Checkers:
	Q_INVOKABLE bool isEnabledAndVisible(const QAction *action) const;

	Q_INVOKABLE bool isEnabledAndVisible(const QWidget *widget) const;

	Q_INVOKABLE bool actionIsChecked(const QAction *action) const;

	Q_INVOKABLE bool actionIsCheckable(const QAction *action) const;

	// Informators:
	/// @note usefull method for debugging and testScripts writing
	Q_INVOKABLE void printPaletteElementsInfo() const;

private:
	void writeIn(QWidget *widget, const QString &lineEditObjectName, const QString &text);
	void clickNeededButton(QWidget *widget, const QString &buttonType, const QString &buttonText);
	void pickNeededItem(QWidget *widget, const QString &comboBoxObjectName, const QString &itemName);
	void clickPushButton(QWidget *widget, const QString &buttonText);
	void clickRadioButton(QWidget *widget, const QString &buttonText);
	void clickCheckBox(QWidget *widget, const QString &buttonText);
	void throwScriptException(const QString &msg);
	void doSmthInWidget(const QString &widgetName, const QString &identifier
						, const QString &buttonText, void (qReal::gui::Utils::*action)(QWidget*, const QString&, const QString &));

	ScriptAPI &mScriptAPI;
	MainWindow &mMainWindow;
	VirtualCursor &mVirtualCursor;
	HintAPI &mHintAPI;
};

}
}
