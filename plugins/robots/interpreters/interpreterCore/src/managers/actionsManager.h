#pragma once

#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtWidgets/QAction>

#include <qrgui/toolPluginInterface/actionInfo.h>
#include <qrgui/toolPluginInterface/hotKeyActionInfo.h>
#include <qrgui/mainwindow/mainWindowInterpretersInterface.h>

#include "kitPluginManager.h"
#include "managers/robotModelManager.h"

namespace interpreterCore {

/// Class that initializes, holds and keeps up-to-date actions added to toolbar and into user menus.
class ActionsManager : public QObject
{
	Q_OBJECT

public:
	/// @todo: Get rid of RobotModelManaged dependence
	ActionsManager(KitPluginManager &kitPluginManager, RobotModelManager &robotModelManager);

	void init(qReal::gui::MainWindowInterpretersInterface *mainWindowInterpretersInterface);

	QList<qReal::ActionInfo> actions();
	QList<qReal::HotKeyActionInfo> hotKeyActionInfos();

	QAction &runAction();

	QAction &stopRobotAction();

	QAction &connectToRobotAction();

	QAction &titlesVisibilityAction();

	QAction &robotSettingsAction();

public slots:
	void onRobotModelChanged(interpreterBase::robotModel::RobotModelInterface &model);
	void onActiveTabChanged(Id const &activeTabId);

private slots:
	void onRobotModelActionChecked(QObject *robotModel);

private:
	QString kitIdOf(interpreterBase::robotModel::RobotModelInterface &model) const;

	/// Updates "enabled" status of plugin actions taking into account current tab, selected robot model and so on.
	void updateEnabledActions();

	/// Loads actions from kit plugins.
	void initKitPluginActions();

	/// Plugins can have their own custom actions, we need to get them from KitPluginManager.
	KitPluginManager &mKitPluginManager;

	/// Actions may switch robot models so we need robot model manager.
	RobotModelManager &mRobotModelManager;

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
	QList<ActionInfo> mPluginActionInfos;  // Does not have ownership over underlying QActions.

	/// Actions that are placed on the panel for quick switching between robot models.
	QMap<QString, ActionInfo> mRobotModelActions;

	QMap<QString, ActionInfo> mGeneratorActionsInfo;  // Does not have ownership over underlying QActions.

	/// List of hotkey customizations from kit plugins.
	QList<HotKeyActionInfo> mPluginHotKeyActionInfos;  // Does not have ownership over underlying QActions.

	/// Main window interface object, to ask about currently open tab and so on.
	// Does not have ownership
	qReal::gui::MainWindowInterpretersInterface *mMainWindowInterpretersInterface = nullptr;
};

}
