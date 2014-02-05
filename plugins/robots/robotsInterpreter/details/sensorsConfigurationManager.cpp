#include "sensorsConfigurationManager.h"

#include <qrkernel/settingsManager.h>

#include <QtCore/QDebug>

using namespace qReal::interpreters::robots::details;

SensorsConfigurationManager::SensorsConfigurationManager()
	: SensorsConfigurationProvider("SensorsConfigurationManager")
{
}

void SensorsConfigurationManager::load()
{
	sensorConfigurationChanged(
			enums::inputPort::port1
			, static_cast<robots::enums::sensorType::SensorTypeEnum>(SettingsManager::value("port1SensorType").toInt())
			);

	sensorConfigurationChanged(
			enums::inputPort::port2
			, static_cast<robots::enums::sensorType::SensorTypeEnum>(SettingsManager::value("port2SensorType").toInt())
			);

	sensorConfigurationChanged(
			enums::inputPort::port3
			, static_cast<robots::enums::sensorType::SensorTypeEnum>(SettingsManager::value("port3SensorType").toInt())
			);

	sensorConfigurationChanged(
			enums::inputPort::port4
			, static_cast<robots::enums::sensorType::SensorTypeEnum>(SettingsManager::value("port4SensorType").toInt())
			);
}

void SensorsConfigurationManager::onSensorConfigurationChanged(
			robots::enums::inputPort::InputPortEnum port
			, robots::enums::sensorType::SensorTypeEnum type
			)
{
	QString const key = portToSettingsKey(port);
	if (SettingsManager::value(key) != type) {
		SettingsManager::setValue(key, type);
	}
}

QString SensorsConfigurationManager::portToSettingsKey(enums::inputPort::InputPortEnum port)
{
	switch (port)
	{
	case enums::inputPort::port1:
		return "port1SensorType";
	case enums::inputPort::port2:
		return "port2SensorType";
	case enums::inputPort::port3:
		return "port3SensorType";
	case enums::inputPort::port4:
		return "port4SensorType";
	default:
		// TODO: Report error somehow.
		qDebug() << "SensorsConfigurationManager::portToSettingsKey: invalid port" << port;
		return "";
	}
}
