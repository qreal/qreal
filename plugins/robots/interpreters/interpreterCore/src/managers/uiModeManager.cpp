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

#include "interpreterCore/managers/uiModeManager.h"

#include <qrkernel/logging.h>
#include <qrkernel/settingsManager.h>

using namespace interpreterCore;

static const QString invalidSettingsKey = "docksInInvalidMode";
static const QString nothingSettingsKey = "docksInNothingMode";
static const QString diagramEditSettingsKey = "docksInDiagramEditMode";
static const QString codeEditSettingsKey = "docksInCodeEditMode";
static const QString debuggingSettingsKey = "docksInDebuggingMode";

UiModeManager::UiModeManager(QAction &debugModeAction
		, QAction &editModeAction
		, qReal::gui::MainWindowDockInterface &mainWindow
		, qReal::SystemEvents &systemEvents
		, kitBase::EventsForKitPluginInterface &kitPluginEvents)
	: mDebugModeAction(debugModeAction)
	, mEditModeAction(editModeAction)
	, mMainWindow(mainWindow)
	, mSystemEvents(systemEvents)
	, mKitPluginEvents(kitPluginEvents)
	, mSettingsKeys({
			{ Mode::Nothing, nothingSettingsKey }
			, { Mode::DiagramEdit, diagramEditSettingsKey }
			, { Mode::CodeEdit, codeEditSettingsKey }
			, { Mode::DiagramEdit, diagramEditSettingsKey }
	})
{
	addWidgetsForDocksDebugging();
}

QString UiModeManager::settingsKeyFor(UiModeManager::Mode mode) const
{
	if (!mSettingsKeys.contains(mode)) {
		QLOG_ERROR() << "No settings key correspond to mode" << static_cast<int>(mode);
		return invalidSettingsKey;
	}

	return mSettingsKeys[mode];
}

void UiModeManager::saveDocks(Mode mode) const
{
	qReal::SettingsManager::setValue(settingsKeyFor(mode), mMainWindow.saveState(static_cast<int>(mode)));
}

void UiModeManager::restoreDocks(UiModeManager::Mode mode) const
{
	const QByteArray state = qReal::SettingsManager::value(settingsKeyFor(mode)).toByteArray();
	if (!mMainWindow.restoreState(state, static_cast<int>(mode))) {
		QLOG_ERROR() << "Cannot apply docks state for mode" << static_cast<int>(mode) << ":" << state;
	}
}

void UiModeManager::addWidgetsForDocksDebugging() const
{
	// This method provides tools only for development.
	// It must not be called in master branch code.
}
