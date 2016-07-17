/* Copyright 2007-2015 QReal Research Group
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
#include <QtXml/QDomDocument>

#include <qrgui/plugins/toolPluginInterface/actionInfo.h>
#include <qrgui/plugins/toolPluginInterface/systemEvents.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/graphicalModelAssistInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/logicalModelAssistInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/mainWindowInterpretersInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/projectManagementInterface.h>
#include <kitBase/devicesConfigurationProvider.h>
#include <kitBase/eventsForKitPluginInterface.h>
#include <kitBase/interpreterControlInterface.h>

#include "twoDModel/twoDModelDeclSpec.h"

namespace twoDModel {

/// An interface for controlling 2D model window.
class TWO_D_MODEL_EXPORT TwoDModelControlInterface : public QObject
{
	Q_OBJECT

public:
	virtual ~TwoDModelControlInterface() {}

	/// Returns a reference to the devices configurator.
	/// Can be used by outside enviroment to connect it to other ones.
	virtual kitBase::DevicesConfigurationProvider &devicesConfigurationProvider() = 0;

	/// Performs 2D model`s initialization with the given system components.
	/// @todo: Separate twoD model engine from the enviroment (get rid of parameters)
	virtual void init(const kitBase::EventsForKitPluginInterface &eventsForKitPlugin
			, const qReal::SystemEvents &systemEvents
			, qReal::LogicalModelAssistInterface &logicalModel
			, qReal::gui::MainWindowInterpretersInterface &interpretersInterface
			, const qReal::ProjectManagementInterface &projectManager
			, kitBase::InterpreterControlInterface &interpreterControl) = 0;

public slots:
	/// Starts interpretation process in 2D model.
	virtual void onStartInterpretation() = 0;

	/// Stops interpretation process in 2D model if started.
	/// @param reason The reason why the interpretation stopped.
	virtual void onStopInterpretation(qReal::interpretation::StopReason reason) = 0;

signals:
	/// Emitted each time when user requests interpretation start from 2D model window.
	void runButtonPressed();

	/// Emitted each time when user requests interpretation stop from 2D model window.
	void stopButtonPressed();
};

}
