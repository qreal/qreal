#include <interpreterBase/robotModel/robotModelUtils.h>

#include <QtXml/QDomDocument>

#include <qrkernel/settingsManager.h>

using namespace interpreterBase::robotModel;

PortInfo RobotModelUtils::findPort(RobotModelInterface const &robotModel, QString const &name, Direction direction)
{
	for (interpreterBase::robotModel::PortInfo const &portInfo : robotModel.availablePorts()) {
		if ((portInfo.name() == name || portInfo.nameAliases().contains(name)) && portInfo.direction() == direction) {
			return portInfo;
		}
	}

	return PortInfo();
}

QMap<QString, QMap<PortInfo, DeviceInfo> > RobotModelUtils::deserialize(QString const &configuration)
{
	QMap<QString, QMap<PortInfo, DeviceInfo>> result;

	QDomDocument parsedConfiguration;
	parsedConfiguration.setContent(configuration);
	QDomElement const rootElement = parsedConfiguration.documentElement();
	for (QDomElement robotModelElement = rootElement.firstChildElement(); !robotModelElement.isNull()
			; robotModelElement = robotModelElement.nextSiblingElement())
	{
		QString const robotModel = robotModelElement.attribute("name");
		for (QDomElement configurationElement = robotModelElement.firstChildElement(); !configurationElement.isNull()
				; configurationElement = configurationElement.nextSiblingElement())
		{
			PortInfo const port = PortInfo::fromString(configurationElement.attribute("port"));
			DeviceInfo const device = DeviceInfo::fromString(configurationElement.attribute("device"));
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

	QString const kitId = kits[0]->kitId();
	QString const key = "SelectedModelFor" + kitId;
	QString const previouslySelectedRobotModel = qReal::SettingsManager::value(key).toString();

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
