#include "actionsManager.h"

#include <qrkernel/settingsManager.h>

using namespace interpreterCore;

static qReal::Id const robotDiagramType = qReal::Id("RobotsMetamodel", "RobotsDiagram", "RobotsDiagramNode");
static qReal::Id const subprogramDiagramType = qReal::Id("RobotsMetamodel", "RobotsDiagram", "SubprogramDiagram");

ActionsManager::ActionsManager(KitPluginManager &kitPluginManager)
	: mRunAction(QIcon(":/icons/robots_run.png"), QObject::tr("Run"), nullptr)
	, mStopRobotAction(QIcon(":/icons/robots_stop.png"), QObject::tr("Stop robot"), nullptr)
	, mConnectToRobotAction(QIcon(":/icons/robots_connect.png"), QObject::tr("Connect to robot"), nullptr)
	, mRobotSettingsAction(QIcon(":/icons/robots_settings.png"), QObject::tr("Robot settings"), nullptr)
	, mTitlesAction(QObject::tr("Text under pictogram"), nullptr)
	, mSeparator1(nullptr)
	, mSeparator2(nullptr)
	, mKitPluginManager(kitPluginManager)
{
	initKitPluginActions();

	mConnectToRobotAction.setCheckable(true);

	mTitlesAction.setCheckable(true);
	mTitlesAction.setChecked(
			qReal::SettingsManager::value("showTitlesForRobots").toBool()
			);

	mSeparator1.setSeparator(true);
	mSeparator2.setSeparator(true);

	mActions
			<< &mRunAction
			<< &mStopRobotAction
			<< &mConnectToRobotAction
			<< &mRobotSettingsAction
			<< &mTitlesAction
			;
}

QList<qReal::ActionInfo> ActionsManager::actions()
{
	QList<qReal::ActionInfo> result;

	result << mPluginActionInfos;
	result << mGeneratorActionsInfo.values();

	result
			<< qReal::ActionInfo(&mRunAction, "interpreters", "tools")
			<< qReal::ActionInfo(&mStopRobotAction, "interpreters", "tools")
			<< qReal::ActionInfo(&mConnectToRobotAction, "interpreters", "tools")
			<< qReal::ActionInfo(&mSeparator1, "interpreters", "tools")
			/// @todo Actions for quick changing of a model shall go here.
			<< qReal::ActionInfo(&mSeparator2, "interpreters", "tools")
			<< qReal::ActionInfo(&mRobotSettingsAction, "interpreters", "tools")
			<< qReal::ActionInfo(&mTitlesAction, "", "settings")
			;

	return result;
}

QList<qReal::HotKeyActionInfo> ActionsManager::hotKeyActionInfos()
{
	mStopRobotAction.setShortcut(QKeySequence(Qt::SHIFT + Qt::Key_F5));
	mRunAction.setShortcut(QKeySequence(Qt::Key_F5));
//	m2dModelAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_2));
	mTitlesAction.setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_T));

	QList<qReal::HotKeyActionInfo> result;

	result += mPluginHotKeyActionInfos;

//	HotKeyActionInfo d2ModelActionInfo("Interpreter.Show2dModel", tr("Show 2d model"), m2dModelAction);
	result
			<< qReal::HotKeyActionInfo("Interpreter.Run", QObject::tr("Run interpreter"), &mRunAction)
			<< qReal::HotKeyActionInfo("Interpreter.Stop", QObject::tr("Stop interpreter"), &mStopRobotAction)

			/// @todo Move it into engine
			<< qReal::HotKeyActionInfo("Editor.ToggleTitles", QObject::tr("Toggle titles visibility"), &mTitlesAction)
			;

	return result;
}

QAction &ActionsManager::runAction()
{
	return mRunAction;
}

QAction &ActionsManager::stopRobotAction()
{
	return mStopRobotAction;
}

QAction &ActionsManager::connectToRobotAction()
{
	return mConnectToRobotAction;
}

void ActionsManager::init(qReal::gui::MainWindowInterpretersInterface *mainWindowInterpretersInterface)
{
	mMainWindowInterpretersInterface = mainWindowInterpretersInterface;

	updateEnabledActions();
}

QAction &ActionsManager::titlesVisibilityAction()
{
	return mTitlesAction;
}

QAction &ActionsManager::robotSettingsAction()
{
	return mRobotSettingsAction;
}

void ActionsManager::onRobotModelChanged(interpreterBase::robotModel::RobotModelInterface &model)
{
	mConnectToRobotAction.setVisible(model.needsConnection());
	QString const currentKitId = kitIdOf(model);

	/// @todo: this stupid visibility management may show actions with custom avalability logic.
	for (QString const &kitId : mKitPluginManager.kitIds()) {
		for (ActionInfo const &actionInfo : mGeneratorActionsInfo.values(kitId)) {
			if (actionInfo.isAction()) {
				actionInfo.action()->setVisible(currentKitId == kitId);
			} else {
				actionInfo.menu()->setVisible(currentKitId == kitId);
			}
		}
	}
}

QString ActionsManager::kitIdOf(interpreterBase::robotModel::RobotModelInterface &model) const
{
	for (QString const &kitId : mKitPluginManager.kitIds()) {
		for (interpreterBase::KitPluginInterface * const kit : mKitPluginManager.kitsById(kitId)) {
			if (kit->robotModels().contains(&model)) {
				return kitId;
			}
		}
	}

	/// @todo: Impossible scenario, something wrong if we get here.
	return QString();
}

void ActionsManager::updateEnabledActions()
{
	qReal::Id const &rootElementId = mMainWindowInterpretersInterface->activeDiagram();
	bool const enabled = rootElementId.type() == robotDiagramType || rootElementId.type() == subprogramDiagramType;

	for (QAction * const action : mActions) {
		action->setEnabled(enabled);
	}
}

void ActionsManager::initKitPluginActions()
{
	for (QString const &kitId : mKitPluginManager.kitIds()) {
		for (interpreterBase::KitPluginInterface * const kitPlugin : mKitPluginManager.kitsById(kitId)) {
			mPluginActionInfos << kitPlugin->customActions();
		}

		for (generatorBase::GeneratorKitPluginInterface * const generator : mKitPluginManager.generatorsById(kitId)) {
			// generator->actions() must be called one so storing it into the field.
			for (ActionInfo const &action : generator->actions()) {
				mGeneratorActionsInfo.insertMulti(kitId, action);
			}

			mPluginHotKeyActionInfos << generator->hotKeyActions();
		}
	}
}
