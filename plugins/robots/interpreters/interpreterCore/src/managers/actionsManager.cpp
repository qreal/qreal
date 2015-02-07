#include "interpreterCore/managers/actionsManager.h"

#include <QtCore/QSignalMapper>

#include <qrkernel/settingsManager.h>

using namespace interpreterCore;

static qReal::Id const robotDiagramType = qReal::Id("RobotsMetamodel", "RobotsDiagram", "RobotsDiagramNode");
static qReal::Id const subprogramDiagramType = qReal::Id("RobotsMetamodel", "RobotsDiagram", "SubprogramDiagram");

ActionsManager::ActionsManager(KitPluginManager &kitPluginManager, RobotModelManager &robotModelManager)
	: mKitPluginManager(kitPluginManager)
	, mRobotModelManager(robotModelManager)
	, mRunAction(QIcon(":/icons/robots_run.png"), QObject::tr("Run"), nullptr)
	, mStopRobotAction(QIcon(":/icons/robots_stop.png"), QObject::tr("Stop robot"), nullptr)
	, mConnectToRobotAction(QIcon(":/icons/robots_connect.png"), QObject::tr("Connect to robot"), nullptr)
	, mRobotSettingsAction(QIcon(":/icons/robots_settings.png"), QObject::tr("Robot settings"), nullptr)
	, mSeparator1(nullptr)
	, mSeparator2(nullptr)
{
	initKitPluginActions();

	mConnectToRobotAction.setCheckable(true);

	mSeparator1.setSeparator(true);
	mSeparator2.setSeparator(true);

	mActions
			<< &mConnectToRobotAction
			<< &mRunAction
			<< &mStopRobotAction
			<< &mRobotSettingsAction
			;
}

QList<qReal::ActionInfo> ActionsManager::actions()
{
	QList<qReal::ActionInfo> result;

	result << mPluginActionInfos;
	result << mGeneratorActionsInfo.values();

	result
			<< qReal::ActionInfo(&mConnectToRobotAction, "interpreters", "tools")
			<< qReal::ActionInfo(&mRunAction, "interpreters", "tools")
			<< qReal::ActionInfo(&mStopRobotAction, "interpreters", "tools")
			<< qReal::ActionInfo(&mSeparator1, "interpreters", "tools");

	result += mRobotModelActions.values();

	result << qReal::ActionInfo(&mSeparator2, "interpreters", "tools")
			<< qReal::ActionInfo(&mRobotSettingsAction, "interpreters", "tools")
			;

	return result;
}

QList<qReal::HotKeyActionInfo> ActionsManager::hotKeyActionInfos()
{
	mStopRobotAction.setShortcut(QKeySequence(Qt::SHIFT + Qt::Key_F5));
	mRunAction.setShortcut(QKeySequence(Qt::Key_F5));

	QList<qReal::HotKeyActionInfo> result;

	result += mPluginHotKeyActionInfos;

	result
			<< qReal::HotKeyActionInfo("Interpreter.Run", QObject::tr("Run interpreter"), &mRunAction)
			<< qReal::HotKeyActionInfo("Interpreter.Stop", QObject::tr("Stop interpreter"), &mStopRobotAction)
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

QAction &ActionsManager::robotSettingsAction()
{
	return mRobotSettingsAction;
}

void ActionsManager::onRobotModelChanged(interpreterBase::robotModel::RobotModelInterface &model)
{
	mConnectToRobotAction.setVisible(model.needsConnection());
	QString const currentKitId = kitIdOf(model);
	QString const switchActionName = "switchTo" + currentKitId + model.name();

	/// @todo: this stupid visibility management may show actions with custom avalability logic.
	for (QString const &kitId : mKitPluginManager.kitIds()) {
		for (qReal::ActionInfo const &actionInfo
				: mRobotModelActions.values(kitId) + mGeneratorActionsInfo.values(kitId))
		{
			if (actionInfo.isAction()) {
				actionInfo.action()->setVisible(currentKitId == kitId);
				actionInfo.action()->setChecked(actionInfo.action()->objectName() == switchActionName);
			} else {
				actionInfo.menu()->setVisible(currentKitId == kitId);
			}
		}
	}
}

void ActionsManager::onActiveTabChanged(qReal::Id const &activeTabId)
{
	bool const isDiagramTab = !activeTabId.isNull();
	mRunAction.setEnabled(isDiagramTab);
	mStopRobotAction.setEnabled(isDiagramTab);
}

void ActionsManager::onRobotModelActionChecked(QObject *robotModelObject)
{
	auto const robotModel = dynamic_cast<interpreterBase::robotModel::RobotModelInterface *>(robotModelObject);
	mRobotModelManager.setModel(robotModel);
	onRobotModelChanged(*robotModel);
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
	QSignalMapper * const robotModelMapper = new QSignalMapper(this);
	connect(robotModelMapper, SIGNAL(mapped(QObject*)), this, SLOT(onRobotModelActionChecked(QObject*)));

	for (QString const &kitId : mKitPluginManager.kitIds()) {
		for (interpreterBase::KitPluginInterface * const kitPlugin : mKitPluginManager.kitsById(kitId)) {
			mPluginActionInfos << kitPlugin->customActions();
			for (interpreterBase::robotModel::RobotModelInterface * const robotModel : kitPlugin->robotModels()) {
				QIcon const &icon = kitPlugin->iconForFastSelector(*robotModel);
				if (icon.isNull()) {
					continue;
				}

				QString const &text = tr("Switch to ") + robotModel->friendlyName();
				QAction * const fastSelectionAction = new QAction(icon, text, nullptr);
				robotModelMapper->setMapping(fastSelectionAction, robotModel);
				connect(fastSelectionAction, SIGNAL(triggered()), robotModelMapper, SLOT(map()));
				fastSelectionAction->setCheckable(true);
				fastSelectionAction->setObjectName("switchTo" + kitId + robotModel->name());
				qReal::ActionInfo const actionInfo(fastSelectionAction, "interpreters", "tools");
				mRobotModelActions.insertMulti(kitId, actionInfo);
			}

			mPluginHotKeyActionInfos << kitPlugin->hotKeyActions();
		}

		for (generatorBase::GeneratorKitPluginInterface * const generator : mKitPluginManager.generatorsById(kitId)) {
			// generator->actions() must be called once so storing it into the field.
			for (qReal::ActionInfo const &action : generator->actions()) {
				mGeneratorActionsInfo.insertMulti(kitId, action);
			}

			mPluginHotKeyActionInfos << generator->hotKeyActions();
		}
	}
}
