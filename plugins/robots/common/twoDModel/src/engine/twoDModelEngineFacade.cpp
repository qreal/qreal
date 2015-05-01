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

#include "twoDModel/engine/twoDModelEngineFacade.h"

#include <qrkernel/logging.h>
#include <qrutils/smartDock.h>
#include <kitBase/readOnly.h>

#include "twoDModel/engine/view/twoDModelWidget.h"
#include "twoDModel/engine/model/model.h"
#include "twoDModelEngineApi.h"

using namespace twoDModel::engine;

TwoDModelEngineFacade::TwoDModelEngineFacade(twoDModel::robotModel::TwoDRobotModel &robotModel)
	: mRobotModelName(robotModel.name())
	, mModel(new model::Model())
	, mView(new view::TwoDModelWidget(*mModel.data()))
	, mApi(new TwoDModelEngineApi(*mModel.data(), *mView.data()))
	, mDock(new utils::SmartDock("2dModelDock", mView.data()))
{
	mModel.data()->addRobotModel(robotModel);

	connect(mView.data(), &view::TwoDModelWidget::runButtonPressed, this, &TwoDModelEngineFacade::runButtonPressed);
	connect(mView.data(), &view::TwoDModelWidget::stopButtonPressed, this, &TwoDModelEngineFacade::stopButtonPressed);
	connect(mView.data(), &view::TwoDModelWidget::widgetClosed, this, &TwoDModelEngineFacade::stopButtonPressed);
	connect(mDock, &utils::SmartDock::dockedChanged, mView.data(), &view::TwoDModelWidget::setCompactMode);
}

TwoDModelEngineFacade::~TwoDModelEngineFacade()
{
}

void TwoDModelEngineFacade::init(const kitBase::EventsForKitPluginInterface &eventsForKitPlugin
		, const qReal::SystemEvents &systemEvents
		, qReal::LogicalModelAssistInterface &logicalModel
		, qReal::gui::MainWindowInterpretersInterface &interpretersInterface
		, kitBase::InterpreterControlInterface &interpreterControl)
{
	mModel->init(*interpretersInterface.errorReporter(), interpreterControl);

	const auto onActiveTabChanged = [this, &logicalModel, &interpretersInterface](const qReal::TabInfo &info)
	{
		mView->setEnabled(info.type() == qReal::TabInfo::TabType::editor);
		const QString xml = logicalModel.logicalRepoApi().metaInformation("worldModel").toString();
		QDomDocument worldModel;
		QString errorMessage;
		int errorLine, errorColumn;
		if (!xml.isEmpty() && !worldModel.setContent(xml, &errorMessage, &errorLine, &errorColumn)) {
			interpretersInterface.errorReporter()->addError(QString("%1:%2: %3")
					.arg(QString::number(errorLine), QString::number(errorColumn), errorMessage));
		}

		mView->loadXml(worldModel);

		loadReadOnlyFlags(logicalModel);
	};

	auto connectTwoDModel = [this, &eventsForKitPlugin, &interpreterControl]()
	{
		connect(&eventsForKitPlugin, &kitBase::EventsForKitPluginInterface::interpretationStarted
				, this, &twoDModel::TwoDModelControlInterface::onStartInterpretation
				, Qt::UniqueConnection);

		connect(&eventsForKitPlugin, &kitBase::EventsForKitPluginInterface::interpretationStopped
				, this, &twoDModel::TwoDModelControlInterface::onStopInterpretation
				, Qt::UniqueConnection);

		connect(this, &twoDModel::TwoDModelControlInterface::runButtonPressed
				, &interpreterControl, &kitBase::InterpreterControlInterface::interpret
				, Qt::UniqueConnection);

		connect(this, &twoDModel::TwoDModelControlInterface::stopButtonPressed
				, &interpreterControl, &kitBase::InterpreterControlInterface::stopRobot
				, Qt::UniqueConnection);
	};

	auto disconnectTwoDModel = [this, &eventsForKitPlugin, &interpreterControl]()
	{
		disconnect(&eventsForKitPlugin, &kitBase::EventsForKitPluginInterface::interpretationStarted
				, this, &twoDModel::TwoDModelControlInterface::onStartInterpretation);

		disconnect(&eventsForKitPlugin, &kitBase::EventsForKitPluginInterface::interpretationStopped
				, this, &twoDModel::TwoDModelControlInterface::onStopInterpretation);

		disconnect(this, &twoDModel::TwoDModelControlInterface::runButtonPressed
				, &interpreterControl, &kitBase::InterpreterControlInterface::interpret);

		disconnect(this, &twoDModel::TwoDModelControlInterface::stopButtonPressed
				, &interpreterControl, &kitBase::InterpreterControlInterface::stopRobot);
	};

	connect(&systemEvents, &qReal::SystemEvents::activeTabChanged, onActiveTabChanged);

	connect(mModel.data(), &model::Model::modelChanged, [this, &logicalModel] (const QDomDocument &xml) {
		logicalModel.mutableLogicalRepoApi().setMetaInformation("worldModel", xml.toString(4));
	});

	connect(&systemEvents, &qReal::SystemEvents::closedMainWindow, [=](){ mView.reset(); delete mDock; });

	connect(&eventsForKitPlugin
			, &kitBase::EventsForKitPluginInterface::robotModelChanged
			, [this, connectTwoDModel, disconnectTwoDModel](const QString &modelName) {
				const bool isCurrentModel = modelName == mRobotModelName;
				if (isCurrentModel) {
					connectTwoDModel();
					mDock->attachToMainWindow(Qt::TopDockWidgetArea);
				} else {
					disconnectTwoDModel();
					mDock->detachFromMainWindow();
				}
			}
			);
}

kitBase::DevicesConfigurationProvider &TwoDModelEngineFacade::devicesConfigurationProvider()
{
	return *mView.data();
}

TwoDModelEngineInterface &TwoDModelEngineFacade::engine()
{
	return *mApi.data();
}

void TwoDModelEngineFacade::onStartInterpretation()
{
	mModel->timeline().start();
}

void TwoDModelEngineFacade::onStopInterpretation()
{
	mModel->timeline().stop();
}

void TwoDModelEngineFacade::loadReadOnlyFlags(const qReal::LogicalModelAssistInterface &logicalModel)
{
	kitBase::ReadOnlyFlags readOnlyFlags = kitBase::ReadOnly::None;

	const auto load = [&] (const QString &tag, kitBase::ReadOnly::ReadOnlyEnum flag) {
		if (logicalModel.logicalRepoApi().metaInformation(tag).toBool()) {
			readOnlyFlags |= flag;
		}
	};

	load("twoDModelWorldReadOnly", kitBase::ReadOnly::World);
	load("twoDModelSensorsReadOnly", kitBase::ReadOnly::Sensors);
	load("twoDModelRobotPositionReadOnly", kitBase::ReadOnly::RobotPosition);
	load("twoDModelRobotConfigurationReadOnly", kitBase::ReadOnly::RobotSetup);
	load("twoDModelSimulationSettingsReadOnly", kitBase::ReadOnly::SimulationSettings);

	mView->setInteractivityFlags(readOnlyFlags);
}
