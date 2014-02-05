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
	explicit ActionsManager(KitPluginManager const &kitPluginManager);

	QList<qReal::ActionInfo> actions();
	QList<qReal::HotKeyActionInfo> hotKeyActionInfos();

	/// \todo Remove this.
	QAction &connectToRobotAction();

	void init(qReal::gui::MainWindowInterpretersInterface *mainWindowInterpretersInterface);

private:
	/// Updates "enabled" status of plugin actions taking into account current tab, selected robot model and so on.
	void updateEnabledActions();

	/// Action that shows 2d model widget
//	QAction *m2dModelAction;

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

	/// List of action infos with plugin actions, for convenient initialization.
	/// Contains all actions which already present as fields.
	QList<QAction *> mActions;

	KitPluginManager const &mKitPluginManager;

	/// Main window interface object, to ask about currently open tab and so on.
	// Does not have ownership
	qReal::gui::MainWindowInterpretersInterface *mMainWindowInterpretersInterface = nullptr;
};

}
