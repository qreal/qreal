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

#include <kitBase/devicesConfigurationProvider.h>

#include <qrgui/plugins/toolPluginInterface/usedInterfaces/graphicalModelAssistInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/logicalModelAssistInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/projectManagementInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/mainWindowInterpretersInterface.h>

namespace interpreterCore {

/// A class that stores device configuration information in a registry and is responsible for synchronizing this
/// information between various places where it can be changed.
/// General architecture for device settings management is this: everybody who has device configuration controls
/// or can alter device configurtion (2d model, DevicesConfigurationWidget, preferences page, actual robot model),
/// or contains them, inherits from DevicesConfigurationProvider and connects to other providers and eventually to
/// DevicesConfigurationManager object (by calling connectDevicesConfigurationProvider method).
///
/// Then any devicesConfigurationChanged calls are propagated through resulting network of interconnected
/// DevicesConfigurationProvider-s so they are kept in sync with each other.
/// DevicesConfigurationManager is a center of this network and is the one place which saves sensor settings
/// in a registry.
class DevicesConfigurationManager : public QObject, public kitBase::DevicesConfigurationProvider
{
	Q_OBJECT

public:
	/// Constructor.
	/// @param graphicalModelAssistInterface - graphical model, needed to save settings as property of current diagram.
	/// @param logicalModelAssistInterface - logical model, also needed to save settings as property of current diagram.
	/// @param mainWindowInterpretersInterface - needed to get current active diagram
	/// @param projectManager - provides notification about changing of currently opened project.
	DevicesConfigurationManager(qReal::GraphicalModelAssistInterface &graphicalModelAssistInterface
			, qReal::LogicalModelAssistInterface &logicalModelAssistInterface
			, qReal::gui::MainWindowInterpretersInterface &mainWindowInterpretersInterface
			, qReal::ProjectManagementInterface &projectManager
			);

private:
	void onDeviceConfigurationChanged(const QString &robotModel
			, const kitBase::robotModel::PortInfo &port
			, const kitBase::robotModel::DeviceInfo &sensor
			, Reason reason) override;

	/// Reacts to opening or closing .qrs project.
	void onOpenedProjectChanged();

	/// Serializes current sensors configuration into inner string representation.
	QString save() const;

	/// Parses given sensors configuration serialized by save() method and broadcasts it to all connected providers.
	void load(const QString &configuration);

	/// Returns the logical id of the robot`s behaviour diagram if it is the only one in save file or Id() otherwise.
	qReal::Id mainDiagramId() const;

	qReal::GraphicalModelAssistInterface &mGraphicalModelAssistInterface;
	qReal::LogicalModelAssistInterface &mLogicalModelAssistInterface;
	qReal::gui::MainWindowInterpretersInterface &mMainWindowInterpretersInterface;
};

}
