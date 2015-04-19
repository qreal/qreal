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
#include <kitBase/eventsForKitPluginInterface.h>

namespace interpreterCore {

/// Manages docks shown in the main window depending on mode user currently works in.
class UiManager : public QObject
{
	Q_OBJECT

public:
	/// Represents the mode user currently works in.
	enum class Mode {
		/// User edits code or diagram or just staring at the start tab or something like this.
		Editing = 0x000001
		/// User debugs the program: interpretation going or 2D model world is constructed.
		, Debugging = 0x000002
	};  // Do not change the values of the elements - it will break backward compability.

	UiManager(QAction &debugModeAction
			, QAction &editModeAction
			, qReal::gui::MainWindowDockInterface &mainWindow
			, qReal::SystemEvents &systemEvents
			, kitBase::EventsForKitPluginInterface &kitPluginEvents);

	/// Embeds the given widget into main window`s left dock panel
	void placeDevicesConfig(QWidget *devicesWidget);

	/// Embeds the given widgets into main window`s left dock panel tabifying them together.
	void placeWatchPlugins(QDockWidget *watchWindow, QWidget *graphicsWatch);

private slots:
	void onActiveTabChanged(const qReal::TabInfo &tab);
	void switchToEditorMode();
	void switchToDebuggerMode();

private:
	void placePluginWindows(QDockWidget *watchWindow, QWidget *sensorsWidget);
	QDockWidget *produceDockWidget(const QString &title, QWidget *content) const;

	int currentMode() const;
	QString currentSettingsKey() const;

	void saveDocks() const;
	void reloadDocks() const;

	void addWidgetsForDocksDebugging() const;

	QAction &mDebugModeAction;
	QAction &mEditModeAction;
	qReal::gui::MainWindowDockInterface &mMainWindow;
	qReal::SystemEvents &mSystemEvents;
	kitBase::EventsForKitPluginInterface &mKitPluginEvents;
	qReal::TabInfo::TabType mCurrentTab = qReal::TabInfo::TabType::other;
	Mode mCurrentMode = Mode::Editing;
};

}
