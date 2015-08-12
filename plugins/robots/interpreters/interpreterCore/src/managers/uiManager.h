/* Copyright 2007-2015 QReal Research Group, Dmitry Mordvinov
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

#include <qrgui/plugins/toolPluginInterface/usedInterfaces/mainWindowDockInterface.h>
#include <qrgui/plugins/toolPluginInterface/systemEvents.h>
#include <kitBase/robotModel/robotModelManagerInterface.h>
#include <kitBase/eventsForKitPluginInterface.h>

namespace qReal {
namespace ui {
class ConsoleDock;
}
}

namespace interpreterCore {

/// Manages docks shown in the main window depending on mode user currently works in.
class UiManager : public QObject
{
	Q_OBJECT

public:
	/// Represents the mode user currently works in.
	enum class Mode {
		/// Neither editing, nor debugging, a value just to store there initial docks dummy state.
		Dummy = 0x000000
		/// User edits code or diagram or just staring at the start tab or something like this.
		, Editing = 0x100000
		/// User debugs the program: interpretation going or 2D model world is constructed.
		, Debugging = 0x200000
	};  // Do not change the values of the elements - it will break backward compability.

	UiManager(QAction &debugModeAction
			, QAction &editModeAction
			, qReal::gui::MainWindowDockInterface &mainWindow
			, qReal::SystemEvents &systemEvents
			, kitBase::EventsForKitPluginInterface &kitPluginEvents
			, kitBase::robotModel::RobotModelManagerInterface &robotModelManager);

	/// Embeds the given widget into main window`s left dock panel
	void placeDevicesConfig(QWidget *devicesWidget);

	/// Embeds the given widgets into main window`s left dock panel tabifying them together.
	void placeWatchPlugins(QDockWidget *watchWindow, QWidget *graphicsWatch);

	/// Appends the \a widget to the end of the custom toolbar. The widget becomes binded to the \robotModel.
	/// It means that \a widget will be shown on the toolbar only when \a robotModel is selected.
	void addWidgetToToolbar(kitBase::robotModel::RobotModelInterface &robotModel, QWidget * const widget);

	/// Returns a pointer to widget where robot standard output is displayed.
	qReal::ui::ConsoleDock &robotConsole();

private slots:
	void onActiveTabChanged(const qReal::TabInfo &tab);
	void onRobotModelChanged(kitBase::robotModel::RobotModelInterface &model);
	void switchToEditorMode();
	void switchToDebuggerMode();
	void switchToMode(Mode mode);
	void toggleModeButtons();

	void saveDocks() const;
	void reloadDocks() const;
	void reloadDocksSavingToolbarsAndErrors() const;
	void resetMainWindowCorners() const;
	void ensureDiagramVisible();

private:
	void initTab();

	QDockWidget *produceDockWidget(const QString &title, QWidget *content) const;
	void produceModeButton(Mode mode, QAction &action, QStatusBar *statusBar) const;

	int currentMode() const;
	QString currentSettingsKey() const;

	void hack2dModelDock() const;
	void enableDocksSnapshotter() const;

	QAction &mDebugModeAction;
	QAction &mEditModeAction;
	qReal::gui::MainWindowDockInterface &mMainWindow;
	QToolBar *mTabBar;  // Takes ownership
	QToolBar *mCustomWidgetsBar;  // Takes ownership.
	qReal::TabInfo::TabType mCurrentTab = static_cast<qReal::TabInfo::TabType>(-1);
	Mode mCurrentMode = Mode::Dummy;
	qReal::ui::ConsoleDock *mRobotConsole;  // Transfers ownership to main window.
	// Takes ownership on keys, doesn`t take on values.
	// QAction manages underlying widgets visibility.
	QMap<QAction *, kitBase::robotModel::RobotModelInterface *> mToolBarWidgets;
};

}
