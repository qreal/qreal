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

#include "interpreterCore/managers/actionsManager.h"

#include <QtCore/QSignalMapper>

#include <qrkernel/settingsManager.h>
#include <kitBase/robotModel/robotModelUtils.h>

using namespace interpreterCore;

static const qReal::Id robotDiagramType = qReal::Id("RobotsMetamodel", "RobotsDiagram", "RobotsDiagramNode");
static const qReal::Id subprogramDiagramType = qReal::Id("RobotsMetamodel", "RobotsDiagram", "SubprogramDiagram");

ActionsManager::ActionsManager(KitPluginManager &kitPluginManager, RobotModelManager &robotModelManager)
	: mKitPluginManager(kitPluginManager)
	, mRobotModelManager(robotModelManager)
	, mRunAction(QIcon(":/icons/robots_run.png"), QObject::tr("Run"), nullptr)
	, mStopRobotAction(QIcon(":/icons/robots_stop.png"), QObject::tr("Stop robot"), nullptr)
	, mConnectToRobotAction(QIcon(":/icons/robots_connect.png"), QObject::tr("Connect to robot"), nullptr)
	, mRobotSettingsAction(QIcon(":/icons/robots_settings.png"), QObject::tr("Robot settings"), nullptr)
	, mExportExerciseAction(QIcon(), QObject::tr("Save as task..."), nullptr)
	, mDebugModeAction(QObject::tr("Switch to debug mode"), nullptr)
	, mEditModeAction(QObject::tr("Switch to edit mode"), nullptr)
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
			<< &mExportExerciseAction
			;

	mEditModeAction.setShortcut(QKeySequence(Qt::CTRL + Qt::Key_1));
	mDebugModeAction.setShortcut(QKeySequence(Qt::CTRL + Qt::Key_2));

	mStopRobotAction.setShortcut(QKeySequence(Qt::SHIFT + Qt::Key_F5));
	mRunAction.setShortcut(QKeySequence(Qt::Key_F5));
}

QList<qReal::ActionInfo> ActionsManager::actions()
{
	QList<qReal::ActionInfo> result;

	result << mPluginActionInfos;

	result
			<< qReal::ActionInfo(&mConnectToRobotAction, "interpreters", "tools")
			<< qReal::ActionInfo(&mRunAction, "interpreters", "tools")
			<< qReal::ActionInfo(&mStopRobotAction, "interpreters", "tools")
			<< qReal::ActionInfo(&mSeparator1, "interpreters", "tools");

	result << mRobotModelActions.values();

	result << qReal::ActionInfo(&mSeparator2, "interpreters", "tools")
			<< qReal::ActionInfo(&mRobotSettingsAction, "interpreters", "tools")
			<< qReal::ActionInfo(&mExportExerciseAction, "", "tools")
			;

	return result;
}

QList<qReal::HotKeyActionInfo> ActionsManager::hotKeyActionInfos()
{
	QList<qReal::HotKeyActionInfo> result;

	result += mPluginHotKeyActionInfos;
	result += mAdditionalHotKeyInfos;

	result
			<< qReal::HotKeyActionInfo("Editor.EditMode", mEditModeAction.text(), &mEditModeAction)
			<< qReal::HotKeyActionInfo("Editor.DebugMode", mDebugModeAction.text(), &mDebugModeAction)
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

QAction &ActionsManager::exportExerciseAction()
{
	return mExportExerciseAction;
}

QAction &ActionsManager::debugModeAction()
{
	return mDebugModeAction;
}

QAction &ActionsManager::editModeAction()
{
	return mEditModeAction;
}

void ActionsManager::appendHotKey(const QString &actionId, const QString &label, QAction &action)
{
	mAdditionalHotKeyInfos << qReal::HotKeyActionInfo(actionId, label, &action);
}

void ActionsManager::onRobotModelChanged(kitBase::robotModel::RobotModelInterface &model)
{
	mConnectToRobotAction.setVisible(model.needsConnection());
	mRunAction.setVisible(model.interpretedModel());
	mStopRobotAction.setVisible(false);
	const QString currentKitId = kitIdOf(model);

	/// @todo: this stupid visibility management may show actions with custom avalability logic.
	for (const QString &kitId : mKitPluginManager.kitIds()) {
		for (const qReal::ActionInfo &actionInfo : mRobotModelActions.values(kitId)) {
			if (actionInfo.isAction()) {
				actionInfo.action()->setVisible(currentKitId == kitId);
			} else {
				actionInfo.menu()->setVisible(currentKitId == kitId);
			}
		}
	}
}

void ActionsManager::onActiveTabChanged(const qReal::TabInfo &info)
{
	updateEnabledActions();
	const bool isDiagramTab = info.type() == qReal::TabInfo::TabType::editor;
	mRunAction.setEnabled(isDiagramTab);
	mStopRobotAction.setEnabled(isDiagramTab);
}

void ActionsManager::onRobotModelActionChecked(QObject *robotModelObject)
{
	const auto robotModel = dynamic_cast<kitBase::robotModel::RobotModelInterface *>(robotModelObject);
	mRobotModelManager.setModel(robotModel);
	onRobotModelChanged(*robotModel);
}

QString ActionsManager::kitIdOf(kitBase::robotModel::RobotModelInterface &model) const
{
	for (const QString &kitId : mKitPluginManager.kitIds()) {
		for (kitBase::KitPluginInterface * const kit : mKitPluginManager.kitsById(kitId)) {
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
	const qReal::Id &rootElementId = mMainWindowInterpretersInterface->activeDiagram();
	const bool enabled = rootElementId.type() == robotDiagramType || rootElementId.type() == subprogramDiagramType;

	for (QAction * const action : mActions) {
		if (action != &mRobotSettingsAction) {
			action->setEnabled(enabled);
		}
	}
}

void ActionsManager::initKitPluginActions()
{
	QSignalMapper * const robotModelMapper = new QSignalMapper(this);
	connect(robotModelMapper, SIGNAL(mapped(QObject*)), this, SLOT(onRobotModelActionChecked(QObject*)));

	for (const QString &kitId : mKitPluginManager.kitIds()) {
		const QList<kitBase::KitPluginInterface *> kits = mKitPluginManager.kitsById(kitId);
		QActionGroup * const group = new QActionGroup(this);
		QList<kitBase::robotModel::RobotModelInterface *> robotModels;
		QMap<kitBase::robotModel::RobotModelInterface *, QIcon> fastSelectorIcons;
		for (kitBase::KitPluginInterface * const kitPlugin : kits) {
			mPluginActionInfos << kitPlugin->customActions();
			mPluginHotKeyActionInfos << kitPlugin->hotKeyActions();
			for (kitBase::robotModel::RobotModelInterface * const robotModel : kitPlugin->robotModels()) {
				const QIcon &icon = kitPlugin->iconForFastSelector(*robotModel);
				if (icon.isNull()) {
					continue;
				}

				fastSelectorIcons[robotModel] = icon;
				robotModels << robotModel;
			}
		}

		QList<QAction *> twoDModelActions;
		QList<QAction *> realRobotActions;
		kitBase::robotModel::RobotModelUtils::sortRobotModels(robotModels);
		for (kitBase::robotModel::RobotModelInterface * const robotModel : robotModels) {
			const QString &text = robotModel->friendlyName();
			QAction * const fastSelectionAction = new QAction(fastSelectorIcons[robotModel], text, nullptr);
			robotModelMapper->setMapping(fastSelectionAction, robotModel);
			connect(fastSelectionAction, SIGNAL(triggered()), robotModelMapper, SLOT(map()));
			fastSelectionAction->setObjectName("switchTo" + kitId + robotModel->name());
			fastSelectionAction->setCheckable(true);
			group->addAction(fastSelectionAction);
			if (text.contains("2D")) {
				twoDModelActions << fastSelectionAction;
			} else {
				realRobotActions << fastSelectionAction;
			}
		}

		if (!kits.isEmpty()) {
			QAction * const realRobotSwitcher = produceMenuAction(kitId, tr("Real robot"), realRobotActions);
			QAction * const twoDModelSwitcher = produceMenuAction(kitId, tr("2D model"), twoDModelActions);

			if (realRobotSwitcher) {
				mRobotModelActions.insertMulti(kitId, qReal::ActionInfo(realRobotSwitcher, "interpreters", "tools"));
			}

			if (twoDModelSwitcher) {
				mRobotModelActions.insertMulti(kitId, qReal::ActionInfo(twoDModelSwitcher, "interpreters", "tools"));
			}
		}
	}
}

QAction *ActionsManager::produceMenuAction(const QString &kitId, const QString &name
		, const QList<QAction *> &subActions) const
{
	if (subActions.isEmpty()) {
		return nullptr;
	}

	if (subActions.count() == 1) {
		QAction * const result = subActions.first();
		connect(&mRobotModelManager, &RobotModelManager::robotModelChanged
				, [this, kitId, result](kitBase::robotModel::RobotModelInterface &model) {
			result->setChecked("switchTo" + kitId + model.name() == result->objectName());
		});

		return result;
	}

	QAction * const menuAction = new QAction(subActions.first()->icon(), name, nullptr);
	menuAction->setCheckable(true);
	menuAction->setMenu(new QMenu);
	menuAction->menu()->addActions(subActions);

	auto checkAction = [this, menuAction, kitId](const QString &name) {
		for (QAction * const action : menuAction->menu()->actions()) {
			if (action->objectName() == name) {
				action->setChecked(true);
				qReal::SettingsManager::setValue("lastFastSelectorActionFor" + kitId, name);
				menuAction->setIcon(action->icon());
				menuAction->setChecked(true);
				return action;
			}
		}

		menuAction->setChecked(false);
		return static_cast<QAction *>(nullptr);
	};

	connect(&mRobotModelManager, &RobotModelManager::robotModelChanged
			, [this, kitId, checkAction](kitBase::robotModel::RobotModelInterface &model) {
		checkAction("switchTo" + kitId + model.name());
	});

	connect(menuAction, &QAction::triggered, [this, kitId, checkAction, menuAction]() {
		const QString key = qReal::SettingsManager::value("lastFastSelectorActionFor" + kitId).toString();
		if (QAction * const action = checkAction(key)) {
			action->trigger();
		} else if (!menuAction->menu()->actions().isEmpty()) {
			menuAction->menu()->actions().first()->trigger();
		}
	});

	return menuAction;
}
