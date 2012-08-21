#pragma once

#include <QtCore/QTranslator>

#include "../../../qrgui/toolPluginInterface/toolPluginInterface.h"
#include "../../../qrgui/toolPluginInterface/pluginConfigurator.h"

#include "interpreter.h"
#include "robotSettingsPage.h"
#include "customizer.h"

namespace qReal {
namespace interpreters {
namespace robots {

class RobotsPlugin : public QObject, public qReal::ToolPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(qReal::ToolPluginInterface)

public:
	RobotsPlugin();
	virtual ~RobotsPlugin();

	virtual void init(PluginConfigurator const &configurator);
	virtual QList<ActionInfo> actions();
	virtual QPair<QString, PreferencesPage *> preferencesPage();
	virtual qReal::Customizer* customizationInterface();
	virtual void updateSettings();
	virtual void closeNeededWidget();

	/// Overriden to enable/disable related actions. For example, we can't run
	/// a diagram which is not related to a plugin.
	virtual void activeTabChanged(Id const & rootElementId);

private slots:
	void showRobotSettings();
	void show2dModel();

private:
	/// Initializes and connects actions, fills action info list
	void initActions();

	/// Disable/enable tab in QList<ActionInfo> info
	void changeActiveTab(QList<ActionInfo> const &info, bool const &trigger);

	/// Tells whether we need to disable or enable particular action on tab change.
	/// For example, we shall be able to access robot settings regardless of currently
	/// open diagram type, but we can't run UML Class diagram as robot program.
	/// @param action Action to be checked
	/// @returns True, if action shall be disabled when current diagram is not robots
	bool needToDisableWhenNotRobotsDiagram(QAction const * const action) const;

	/// Customizer object for this plugin
	Customizer mCustomizer;

	/// Main class for robot interpreter. Contains implementation of plugin functionality.
	Interpreter mInterpreter;

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

	/// Action that shows watch list that allows to see current values of variables in a program
	/// and current sensor readings
	QAction *mWatchListAction;

	/// List of action infos with plugin actions, for convenient initialization.
	/// Contains all actions which already present as fields.
	QList<qReal::ActionInfo> mActionInfos;

	/// Plugin translator object
	QTranslator *mAppTranslator;  // Has ownership
};

}
}
}
