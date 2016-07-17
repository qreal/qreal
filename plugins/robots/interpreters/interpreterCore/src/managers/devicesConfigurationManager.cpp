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

#include "interpreterCore/managers/devicesConfigurationManager.h"

#include <QtXml/QDomDocument>

#include <qrkernel/settingsManager.h>
#include <kitBase/robotModel/robotModelUtils.h>

using namespace interpreterCore;
using namespace kitBase;
using namespace robotModel;
using namespace qReal;

const QString diagramName = "RobotsDiagramNode";

DevicesConfigurationManager::DevicesConfigurationManager(
		qReal::GraphicalModelAssistInterface &graphicalModelAssistInterface
		, qReal::LogicalModelAssistInterface &logicalModelAssistInterface
		, qReal::gui::MainWindowInterpretersInterface &mainWindowInterpretersInterface
		, qReal::ProjectManagementInterface &projectManager
		)
	: DevicesConfigurationProvider("DevicesConfigurationManager")
	, mGraphicalModelAssistInterface(graphicalModelAssistInterface)
	, mLogicalModelAssistInterface(logicalModelAssistInterface)
	, mMainWindowInterpretersInterface(mainWindowInterpretersInterface)
{
	connect(&projectManager, &qReal::ProjectManagementInterface::afterOpen
			, this, &DevicesConfigurationManager::onOpenedProjectChanged);
	connect(&projectManager, &qReal::ProjectManagementInterface::closed
			, this, &DevicesConfigurationManager::onOpenedProjectChanged);
}

QString DevicesConfigurationManager::save() const
{
	QDomDocument result;
	QDomElement rootElement = result.createElement("devicesConfiguration");
	result.appendChild(rootElement);
	for (const QString &robotModel : configuredModels()) {
		QDomElement robotModelElement = result.createElement("robotModel");
		robotModelElement.setAttribute("name", robotModel);
		rootElement.appendChild(robotModelElement);
		for (const PortInfo &port : configuredPorts(robotModel)) {
			const DeviceInfo device = currentConfiguration(robotModel, port);
			QDomElement configurationElement = result.createElement("configuration");
			configurationElement.setAttribute("port", port.toString());
			configurationElement.setAttribute("device", device.toString());
			robotModelElement.appendChild(configurationElement);
		}
	}

	return result.toString();
}

void DevicesConfigurationManager::load(const QString &configuration)
{
	clearConfiguration(Reason::loading);

	QMap<QString, QMap<PortInfo, DeviceInfo>> const parsed = RobotModelUtils::deserialize(configuration);
	for (const QString &robotModel : parsed.keys()) {
		for (const PortInfo &port : parsed[robotModel].keys()) {
			deviceConfigurationChanged(robotModel, port, parsed[robotModel][port], Reason::loading);
		}
	}
}

Id DevicesConfigurationManager::mainDiagramId() const
{
	Id result;
	const IdList diagrams = mGraphicalModelAssistInterface.children(Id::rootId());
	for (const Id &logicalDiagramId : diagrams) {
		if (logicalDiagramId.element() == diagramName && mLogicalModelAssistInterface.isLogicalId(logicalDiagramId)) {
			if (!result.isNull()) {
				// Then there are more than two robot diagrams in this save, ignoring all of them...
				return Id();
			}

			result = logicalDiagramId;
		}
	}

	return result;
}

void DevicesConfigurationManager::onDeviceConfigurationChanged(const QString &robotModel
		, const PortInfo &port, const DeviceInfo &sensor, Reason reason)
{
	if (reason == Reason::loading) {
		return;
	}

	Q_UNUSED(robotModel)
	Q_UNUSED(port)
	Q_UNUSED(sensor)

	const qReal::Id activeDiagramGraphicalId = mMainWindowInterpretersInterface.activeDiagram();
	const qReal::Id logicalRootId = activeDiagramGraphicalId.element() != diagramName
			? mainDiagramId()
			: mGraphicalModelAssistInterface.logicalId(activeDiagramGraphicalId);
	if (logicalRootId.isNull()) {
		return;
	}

	mLogicalModelAssistInterface.setPropertyByRoleName(logicalRootId, save(), "devicesConfiguration");
}

void DevicesConfigurationManager::onOpenedProjectChanged()
{
	const Id logicalRootId = mGraphicalModelAssistInterface.logicalId(mMainWindowInterpretersInterface.activeDiagram());
	if (logicalRootId.element() != diagramName) {
		return;
	}

	const QString devicesConfiguration = logicalRootId.isNull()
			? QString()
			: mLogicalModelAssistInterface.propertyByRoleName(logicalRootId, "devicesConfiguration").toString();
	load(devicesConfiguration);
}
