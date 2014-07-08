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
	/// Constructor.
	/// @param kitPluginManager - information about currently loaded kit plugins.
	/// @param robotModelManager - needed to be able to switch models directly from ActionsManager.
	/// @todo: Get rid of RobotModelManager dependence
	ActionsManager(KitPluginManager &kitPluginManager, RobotModelManager &robotModelManager);

	/// Initializes this manager and sets "enabled" status for its actions.
	void init(qReal::gui::MainWindowInterpretersInterface *mainWindowInterpretersInterface);

	/// Returns list of currently available actions (including kit- and model-specific).
	QList<qReal::ActionInfo> actions();

	/// Returns hotkeys for available actions.
	/// @todo Why we can't store hotkey information inside ActionInfo?
	QList<qReal::HotKeyActionInfo> hotKeyActionInfos();

	/// Start interpretation action.
	/// @todo Does nothing on real TRIK model, need to connect it to corresponding generator action somehow.
	QAction &runAction();

	/// Stop interpretation.
	/// @todo Also confusingly has nothing to do with "stop robot" generator action on TRIK.
	QAction &stopRobotAction();

	/// Action for establishing connection to a robot.
	QAction &connectToRobotAction();

	/// Action for showing or hiding labels of blocks on scene.
	/// @todo Move this action to QReal core.
	QAction &titlesVisibilityAction();

	/// Action that shows settings page with "robots" tab selected.
	QAction &robotSettingsAction();

public slots:
	/// Reacts to selection of another robot model.
	/// @param model - newly selected robot model.
	void onRobotModelChanged(interpreterBase::robotModel::RobotModelInterface &model);

	/// Reacts to selection of a new tab on scene.
	/// @param activeTabId - id of a root element of new active tab.
	void onActiveTabChanged(Id const &activeTabId);

private slots:
	void onRobotModelActionChecked(QObject *robotModelObject);

private:
	QString kitIdOf(interpreterBase::robotModel::RobotModelInterface &model) const;

	/// Updates "enabled" status of plugin actions taking into account current tab, selected robot model and so on.
	void updateEnabledActions();

	/// Loads actions from kit plugins.
	void initKitPluginActions();

	///Give object names to actions
	void giveObjectNames();

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
