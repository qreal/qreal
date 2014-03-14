#pragma once

#include <QtCore/QList>
#include <QtWidgets/QAction>
#include <qrgui/toolPluginInterface/actionInfo.h>
#include <qrgui/toolPluginInterface/hotKeyActionInfo.h>
#include <qrgui/mainwindow/mainWindowInterpretersInterface.h>

#include "kitPluginManager.h"

namespace interpreterCore {

/// Class that initializes, holds and keeps up-to-date actions added to toolbar and into user menus.
class ActionsManager
{
public:
	explicit ActionsManager(KitPluginManager &kitPluginManager);

	void init(qReal::gui::MainWindowInterpretersInterface *mainWindowInterpretersInterface);

	QList<qReal::ActionInfo> actions();
	QList<qReal::HotKeyActionInfo> hotKeyActionInfos();

	QAction &runAction();

	QAction &stopRobotAction();

	QAction &connectToRobotAction();

	QAction &titlesVisibilityAction();

	QAction &robotSettingsAction();

private:
	/// Updates "enabled" status of plugin actions taking into account current tab, selected robot model and so on.
	void updateEnabledActions();

	/// Loads actions from kit plugins.
	void initKitPluginActions();

	/// Action that runs program
	QAction mRunAction;

	/// Action that stops program and also stops robot motors
	QAction mStopRobotAction;

	/// Checkable action that establishes connection to robot. If successful,
	/// action will be checked, if connection lost, it will uncheck
	QAction mConnectToRobotAction;

	/// Action that shows robots tab in settings dialog
	QAction mRobotSettingsAction;

	/// Action that shows or hides titles on diagram
	QAction mTitlesAction;

	QAction mSeparator1;
	QAction mSeparator2;

	/// List of plugin actions, for convenient initialization. Contains all actions which already present as fields.
	QList<QAction *> mActions;  // Does not have ownership (actions already present as fields).

	/// List of actions from kit plugins.
	QList<qReal::ActionInfo> mPluginActionInfos;  // Does not have ownership over underlying QActions.

	/// Plugins can have their own custom actions, we need to get them from KitPluginManager.
	KitPluginManager &mKitPluginManager;

	/// Main window interface object, to ask about currently open tab and so on.
	// Does not have ownership
	qReal::gui::MainWindowInterpretersInterface *mMainWindowInterpretersInterface = nullptr;
};

}
