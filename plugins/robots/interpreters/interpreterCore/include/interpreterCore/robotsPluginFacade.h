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

#include <QtCore/QObject>
#include <QtCore/QScopedPointer>

#include <qrgui/plugins/toolPluginInterface/pluginConfigurator.h>
#include <kitBase/eventsForKitPluginInterface.h>
#include <kitBase/devicesConfigurationWidget.h>

#include "interpreterCore/customizer.h"
#include "interpreterCore/managers/kitPluginManager.h"
#include "interpreterCore/managers/robotModelManager.h"
#include "interpreterCore/managers/actionsManager.h"
#include "interpreterCore/managers/devicesConfigurationManager.h"
#include "interpreterCore/managers/graphicsWatcherManager.h"
#include "interpreterCore/managers/blocksFactoryManager.h"
#include "interpreter/interpreter.h"
#include "textLanguage/robotsBlockParser.h"

namespace interpreterCore {

class ExerciseExportManager;
class UiManager;

namespace ui {
class RobotsSettingsPage;
}

/// Responsible for initialization, interconnecting and keeping in sync core plugin subsystems.
class RobotsPluginFacade : public QObject
{
	Q_OBJECT

public:
	RobotsPluginFacade();
	~RobotsPluginFacade() override;

	/// Inits all sybsytems of robots plugin infrastructure that somehow depend from engine`s parts.
	void init(const qReal::PluginConfigurator &configurer);

	/// Returns a pointer to settings page for robots plugin.
	qReal::gui::PreferencesPage *robotsSettingsPage() const;  // Transfers ownership.

	/// Returns the customization class object for configuring QReal's main GUI module
	Customizer &customizer();

	/// Returns an object of the class responsible for managing plugin`s toolbar actions.
	ActionsManager &actionsManager();

	/// A convenience method that travels around all loaded kit plugins,
	/// collects all non-empty default settings file paths and returns them.
	QStringList defaultSettingsFiles() const;

	/// Returns a helper object for convenient 2D model interface scripting.
	QObject *guiScriptFacade() const;

	/// Returns diagram interpter`s management interface.
	interpreter::InterpreterInterface &interpreter() const;

private:
	void connectInterpreterToActions();

	/// @returns true, if kit selection successful, false when no kit plugins are loaded.
	bool selectKit(const qReal::PluginConfigurator &configurer);

	void initSensorWidgets();

	void initKitPlugins(const qReal::PluginConfigurator &configurer);

	void initFactoriesFor(const QString &kitId
			, const kitBase::robotModel::RobotModelInterface *model
			, const qReal::PluginConfigurator &configurer);

	void connectEventsForKitPlugin();

	/// After all parts of a plugin are connected to each other, sends notifications about changes which were missed
	/// during initialization process. For example, model change notification is sent in constructor of settings page,
	/// before kit plugins were even created, so we need to resend it.
	void sync();

	/// Customizer object for this plugin
	Customizer mCustomizer;

	QScopedPointer<textLanguage::RobotsBlockParser> mParser;

	/// Main class for robot interpreter. Contains implementation of generic diagram interpreter.
	interpreter::InterpreterInterface *mInterpreter;  // Has ownership

	/// Page with plugin settings. Created here, but then ownership is passed to a caller of preferencesPage().
	ui::RobotsSettingsPage *mRobotSettingsPage;  // Does not have ownership

	KitPluginManager mKitPluginManager;
	RobotModelManager mRobotModelManager;
	ActionsManager mActionsManager;
	QScopedPointer<DevicesConfigurationManager> mDevicesConfigurationManager;
	QScopedPointer<ExerciseExportManager> mSaveAsTaskManager;
	QScopedPointer<UiManager> mUiManager;

	kitBase::DevicesConfigurationWidget *mDockDevicesConfigurer;  // Does not have ownership
	utils::WatchListWindow *mWatchListWindow;  // Does not have ownership
	GraphicsWatcherManager *mGraphicsWatcherManager;  // Has ownership
	BlocksFactoryManager mBlocksFactoryManager;
	kitBase::EventsForKitPluginInterface mEventsForKitPlugin;
};

}
