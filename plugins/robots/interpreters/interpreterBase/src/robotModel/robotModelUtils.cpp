#include <interpreterBase/robotModel/robotModelUtils.h>

#include <QtXml/QDomDocument>

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
