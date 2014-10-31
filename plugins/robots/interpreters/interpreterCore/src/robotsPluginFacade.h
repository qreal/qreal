#pragma once

#include <QtCore/QObject>
#include <QtCore/QScopedPointer>

#include <qrgui/plugins/toolPluginInterface/pluginConfigurator.h>
#include <interpreterBase/eventsForKitPluginInterface.h>
#include <interpreterBase/devicesConfigurationWidget.h>

#include "customizer.h"
#include "managers/kitPluginManager.h"
#include "managers/robotModelManager.h"
#include "managers/actionsManager.h"
#include "managers/devicesConfigurationManager.h"
#include "managers/graphicsWatcherManager.h"
#include "managers/blocksFactoryManager.h"
#include "interpreter/interpreter.h"
#include "textLanguage/robotsBlockParser.h"
#include "ui/robotsSettingsPage.h"
#include "commonTwoDModel/engine/twoDModelGuiFacade.h"

namespace interpreterCore {

/// Responsible for initialization, interconnecting and keeping in sync core plugin subsystems.
class RobotsPluginFacade : public QObject
{
	Q_OBJECT

public:
	RobotsPluginFacade();

	~RobotsPluginFacade() override;

	/// Inits all sybsytems of robots plugin infrastructure that somehow depend from engine`s parts.
	void init(qReal::PluginConfigurator const &configurer);

	/// Returns a pointer to settings page for robots plugin.
	qReal::gui::PreferencesPage *robotsSettingsPage() const;  // Transfers ownership.

	/// Returns the customization class object for configuring QReal's main GUI module
	Customizer &customizer();

	/// Returns an object of the class responsible for managing plugin`s toolbar actions.
	ActionsManager &actionsManager();

	/// A convenience method that travels around all loaded kit plugins,
	/// collects all non-empty default settings file pathes and returns them.
	QStringList defaultSettingsFiles() const;

	///Returns 2d model gui script facade
	twoDModel::engine::TwoDModelGuiFacade *guiScriptFacade();

private:
	void connectInterpreterToActions();

	/// @returns true, if kit selection successful, false when no kit plugins are loaded.
	bool selectKit(qReal::PluginConfigurator const &configurer);

	void initSensorWidgets();

	void initKitPlugins(qReal::PluginConfigurator const &configurer);

	void initFactoriesFor(QString const &kitId
			, interpreterBase::robotModel::RobotModelInterface const *model
			, qReal::PluginConfigurator const &configurer);

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

	interpreterBase::DevicesConfigurationWidget *mDockDevicesConfigurer;  // Does not have ownership
	utils::WatchListWindow *mWatchListWindow;  // Does not have ownership
	GraphicsWatcherManager *mGraphicsWatcherManager;  // Has ownership

	BlocksFactoryManager mBlocksFactoryManager;

	interpreterBase::EventsForKitPluginInterface mEventsForKitPlugin;
};

}
