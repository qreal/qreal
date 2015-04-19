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
class UiModeManager
{
public:
	/// Represents the mode user currently works in.
	enum class Mode {
		/// User is doing something other that editing diagrams or code (for example start tab is opened).
		Nothing = 0
		/// User edits visual diagram (diagram tab is opened and interpretation is not going).
		, DiagramEdit
		/// User edits code (text tab is opened and interpretation is not going).
		, CodeEdit
		/// User edits diagram or code and interpretation going.
		, Debugging
	};

	UiModeManager(QAction &debugModeAction
			, QAction &editModeAction
			, qReal::gui::MainWindowDockInterface &mainWindow
			, qReal::SystemEvents &systemEvents
			, kitBase::EventsForKitPluginInterface &kitPluginEvents);

private:
	QString settingsKeyFor(Mode mode) const;
	void saveDocks(Mode mode) const;
	void restoreDocks(Mode mode) const;

	void addWidgetsForDocksDebugging() const;

	QAction &mDebugModeAction;
	QAction &mEditModeAction;
	qReal::gui::MainWindowDockInterface &mMainWindow;
	qReal::SystemEvents &mSystemEvents;
	kitBase::EventsForKitPluginInterface &mKitPluginEvents;
	const QHash<Mode, QString> mSettingsKeys;
};

uint qHash(UiModeManager::Mode mode)
{
	return static_cast<uint>(mode);
}

}
