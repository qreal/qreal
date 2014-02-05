#include "sensorsConfigurationManager.h"

#include <QtXml/QDomDocument>

#include <qrkernel/settingsManager.h>

using namespace interpreterCore;
using namespace interpreterBase;
using namespace robotModel;
using namespace qReal;

SensorsConfigurationManager::SensorsConfigurationManager()
	: SensorsConfigurationProvider("SensorsConfigurationManager")
{
}

QString SensorsConfigurationManager::save() const
{
	QDomDocument result;
	for (QString const &robotModel : mCurrentConfiguration.keys()) {
		QDomElement robotModelElement = result.createElement("robotModel");
		robotModelElement.setAttribute("name", robotModel);
		result.appendChild(robotModelElement);
		for (PortInfo const &port : mCurrentConfiguration[robotModel].keys()) {
			PluggableDeviceInfo const device = mCurrentConfiguration[robotModel][port];
			QDomElement configurationElement = result.createElement("configuration");
			configurationElement.setAttribute("port", port.toString());
			configurationElement.setAttribute("device", device.toString());
			robotModelElement.appendChild(configurationElement);
		}
	}

	return result.toString();
}

void SensorsConfigurationManager::load(QString const &configuration)
{
	mCurrentConfiguration.clear();
	QDomDocument parsedConfiguration;
	parsedConfiguration.setContent(configuration);
	for (QDomElement robotModelElement = parsedConfiguration.firstChildElement(); !robotModelElement.isNull()
			; robotModelElement = robotModelElement.nextSiblingElement())
	{
		QString const robotModel = robotModelElement.attribute("name");
		for (QDomElement configurationElement = parsedConfiguration.firstChildElement(); !configurationElement.isNull()
				; configurationElement = configurationElement.nextSiblingElement())
		{
			PortInfo const port = PortInfo::fromString(robotModelElement.attribute("port"));
			PluggableDeviceInfo const device = PluggableDeviceInfo::fromString(robotModelElement.attribute("device"));
			if (port.isValid()) {
				mCurrentConfiguration[robotModel][port] = device;
			}
		}
	}

	refreshSensorsConfiguration();
}

void SensorsConfigurationManager::onSensorConfigurationChanged(QString const &robotModel
		, PortInfo const &port, PluggableDeviceInfo const &sensor)
{
//	QString const key = portToSettingsKey(port);
//	if (SettingsManager::value(key) != sensor) {
//		SettingsManager::setValue(key, sensor);
//	}
}

QString SensorsConfigurationManager::portToSettingsKey(PortInfo const &port)
{
//	switch (port)
//	{
//	case enums::inputPort::port1:
//		return "port1SensorType";
//	case enums::inputPort::port2:
//		return "port2SensorType";
//	case enums::inputPort::port3:
//		return "port3SensorType";
//	case enums::inputPort::port4:
//		return "port4SensorType";
//	default:
//		return QString();
//	}
}
