#pragma once

#include <QtCore/QTranslator>

#include "../../../qrgui/toolPluginInterface/toolPluginInterface.h"
#include "../../../qrgui/toolPluginInterface/pluginConfigurator.h"
#include "../../../qrgui/toolPluginInterface/hotKeyActionInfo.h"

#include "robotSettingsPage.h"
#include "customizer.h"
#include "details/interpreter.h"
#include "details/sensorsConfigurationWidget.h"
#include "details/sensorsConfigurationManager.h"
#include "details/nxtDisplay.h"

namespace qReal {
namespace interpreters {
namespace robots {

class RobotsPlugin : public QObject, public qReal::ToolPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(qReal::ToolPluginInterface)
	Q_PLUGIN_METADATA(IID "qReal.robots.interpreters.robots.RobotsPlugin")

public:
	RobotsPlugin();
	virtual ~RobotsPlugin();

	virtual void init(PluginConfigurator const &configurator);
	virtual QList<ActionInfo> actions();
	virtual QList<HotKeyActionInfo> hotKeyActions();
	virtual QPair<QString, PreferencesPage *> preferencesPage();
	virtual qReal::Customizer* customizationInterface();

private slots:
	void showRobotSettings();
	void show2dModel();
	void rereadSettings();
	void setModelType(int type);
	void setGraphWatcherSettings();
	void titlesVisibilityCheckedInPlugin(bool checked);
	void titlesVisibilityChecked(bool checked);
	void updateSettings();
	void closeNeededWidget();

	/// Overriden to enable/disable related actions. For example, we can't run
	/// a diagram which is not related to a plugin.
	void activeTabChanged(Id const &rootElementId);

private:
	/// Initializes and connects actions, fills action info list
	void initActions();

	void initHotKeyActions();

	void updateTitlesVisibility();

	/// Updates "enabled" status of interpreter actions taking into account current tab,
	/// selected robot model and so on.
	void updateEnabledActions();

	void reinitModelType();

	details::SensorsConfigurationWidget *produceSensorsConfigurer();

	/// Customizer object for this plugin
	Customizer mCustomizer;

	/// Main class for robot interpreter. Contains implementation of plugin functionality.
	details::Interpreter *mInterpreter; // Has ownership

	/// Page with plugin settings. Created here, but then ownership is passed to
	/// a caller of preferencesPage().
	PreferencesRobotSettingsPage *mRobotSettingsPage;  // Does not have ownership

	/// Main window interface object, used to call GUI functionality
	gui::MainWindowInterpretersInterface *mMainWindowInterpretersInterface;  // Does not have ownership

	/// Action that shows 2d model widget
	QAction *m2dModelAction;

	/// Action that runs program
	QAction *mRunAction;

	/// Action that stops program and also stops robot motors
	QAction *mStopRobotAction;

	/// Checkable action that establishes connection to robot. If successful,
	/// action will be checked, if connection lost, it will uncheck
	QAction *mConnectToRobotAction;

	/// Action that shows robots tab in settings dialog
	QAction *mRobotSettingsAction;

	/// Action that switches current robot model to unreal one
	QAction *mSwitchTo2DModelAction;

	/// Action that switches current robot model to nxt one
	QAction *mSwitchToNxtModelAction;

	/// Action that switches current robot model to trik one
	QAction *mSwitchToTrikModelAction;

	/// Action that shows or hides titles on diagram
	QAction *mTitlesAction;

	/// List of action infos with plugin actions, for convenient initialization.
	/// Contains all actions which already present as fields.
	QList<qReal::ActionInfo> mActionInfos;

	QList<HotKeyActionInfo> mHotKeyActionInfos;

	/// Plugin translator object
	QTranslator *mAppTranslator;  // Has ownership

	SceneCustomizationInterface *mSceneCustomizer;  // Does not have ownership

	details::SensorsConfigurationManager mSensorsConfigurationManager;
};

}
}
}
