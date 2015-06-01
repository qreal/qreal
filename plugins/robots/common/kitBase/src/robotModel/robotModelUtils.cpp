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

#include <kitBase/robotModel/robotModelUtils.h>

#include <QtXml/QDomDocument>

#include <qrkernel/settingsManager.h>

using namespace kitBase::robotModel;

PortInfo RobotModelUtils::findPort(const RobotModelInterface &robotModel, const QString &name, Direction direction)
{
	for (const kitBase::robotModel::PortInfo &portInfo : robotModel.availablePorts()) {
		if ((portInfo.name() == name || portInfo.nameAliases().contains(name)) && portInfo.direction() == direction) {
			return portInfo;
		}
	}

	return PortInfo();
}

QMap<QString, QMap<PortInfo, DeviceInfo> > RobotModelUtils::deserialize(const QString &configuration)
{
	QMap<QString, QMap<PortInfo, DeviceInfo>> result;

	QDomDocument parsedConfiguration;
	parsedConfiguration.setContent(configuration);
	const QDomElement rootElement = parsedConfiguration.documentElement();
	for (QDomElement robotModelElement = rootElement.firstChildElement(); !robotModelElement.isNull()
			; robotModelElement = robotModelElement.nextSiblingElement())
	{
		const QString robotModel = robotModelElement.attribute("name");
		for (QDomElement configurationElement = robotModelElement.firstChildElement(); !configurationElement.isNull()
				; configurationElement = configurationElement.nextSiblingElement())
		{
			const PortInfo port = PortInfo::fromString(configurationElement.attribute("port"));
			const DeviceInfo device = DeviceInfo::fromString(configurationElement.attribute("device"));
			if (port.isValid()) {
				result[robotModel][port] = device;
			}
		}
	}

	return result;
}

RobotModelInterface *RobotModelUtils::selectedRobotModelFor(QList<KitPluginInterface *> const &kits)
{
	if (kits.empty()) {
		return nullptr;
	}

	const QString kitId = kits[0]->kitId();
	const QString key = "SelectedModelFor" + kitId;
	const QString previouslySelectedRobotModel = qReal::SettingsManager::value(key).toString();

	if (!previouslySelectedRobotModel.isEmpty()) {
		// Searching for previously selected robot model in the kit.
		for (KitPluginInterface * const kitPlugin : kits) {
			for (RobotModelInterface * const robotModel : kitPlugin->robotModels()) {
				if (robotModel->name() == previouslySelectedRobotModel) {
					return robotModel;
				}
			}
		}
	}

	// Robot model for this kit was never selected or previous selection was not found in the kit.
	// Falling back to kit`s default robot model.
	for (KitPluginInterface * const kitPlugin : kits) {
		if (RobotModelInterface * const kitDefaultRobotModel = kitPlugin->defaultRobotModel()) {
			return kitDefaultRobotModel;
		}
	}

	// No robot model was selected previously and no default robot model was specified.
	// Falling back to first met robot model.
	for (KitPluginInterface * const kitPlugin : kits) {
		if (!kitPlugin->robotModels().isEmpty()) {
			return kitPlugin->robotModels()[0];
		}
	}

	// No robot models found in the kit.
	return nullptr;
}

void RobotModelUtils::sortRobotModels(QList<RobotModelInterface *> &robotModels)
{
	qSort(robotModels.begin(), robotModels.end()
			, [](RobotModelInterface * const robot1, RobotModelInterface * const robot2) {
		return robot1->priority() > robot2->priority();
	});
}
