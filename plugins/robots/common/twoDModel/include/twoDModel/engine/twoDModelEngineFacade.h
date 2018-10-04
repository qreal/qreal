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

#pragma once

#include <QtCore/QString>
#include <QtCore/QScopedPointer>

#include "twoDModel/robotModel/twoDRobotModel.h"
#include "twoDModel/engine/twoDModelControlInterface.h"
#include "twoDModel/engine/twoDModelEngineInterface.h"

#include "twoDModel/twoDModelDeclSpec.h"

namespace utils {
class SmartDock;
}

namespace twoDModel {

namespace model {
class Model;
}
namespace view {
class TwoDModelWidget;
}

namespace engine {

/// Manages all 2D model engine combining components together and provides API for controling it.
class TWO_D_MODEL_EXPORT TwoDModelEngineFacade : public TwoDModelControlInterface
{
	Q_OBJECT

public:
	/// @param configurer - allows to configure various model parameters specific to a kit. Takes ownership.
	explicit TwoDModelEngineFacade(twoDModel::robotModel::TwoDRobotModel &robotModel);

	~TwoDModelEngineFacade() override;

	void init(const kitBase::EventsForKitPluginInterface &eventsForKitPlugin
			, const qReal::SystemEvents &systemEvents
			, qReal::LogicalModelAssistInterface &logicalModel
			, qReal::ControllerInterface &controller
			, qReal::gui::MainWindowInterpretersInterface &interpretersInterface
			, qReal::gui::MainWindowDockInterface &dockInterface
			, const qReal::ProjectManagementInterface &projectManager
			, kitBase::InterpreterControlInterface &interpreterControl) override;

	kitBase::DevicesConfigurationProvider &devicesConfigurationProvider() override;

	TwoDModelEngineInterface &engine();

public slots:
	void onStartInterpretation() override;
	void onStopInterpretation(qReal::interpretation::StopReason reason) override;

private:
	void loadReadOnlyFlags(const qReal::LogicalModelAssistInterface &logicalModel);

	const QString mRobotModelName;

	QScopedPointer<model::Model> mModel;
	QScopedPointer<view::TwoDModelWidget> mView;
	QScopedPointer<TwoDModelEngineInterface> mApi;
	utils::SmartDock *mDock;  // Transfers ownership to main window indirectly

	qReal::TabInfo::TabType mCurrentTabInfo; // temp hack
};

}
}
