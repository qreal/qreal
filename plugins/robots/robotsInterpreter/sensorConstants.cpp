#include "sensorConstants.h"

using namespace qReal::interpreters::robots;

QStringList SensorEnumerator::sensorNamesList()
{
	return QStringList()
			<< QObject::tr("Unused")
			<< QObject::tr("Touch sensor (boolean value)")
			<< QObject::tr("Touch sensor (raw value)")
			<< QObject::tr("Sonar sensor")
			<< QObject::tr("Light sensor")
			<< QObject::tr("Color sensor (full colors)")
			<< QObject::tr("Color sensor (red)")
			<< QObject::tr("Color sensor (green)")
			<< QObject::tr("Color sensor (blue)")
			<< QObject::tr("Color sensor (passive)")
			<< QObject::tr("Sound sensor")
			<< QObject::tr("Gyroscope");
}

QString SensorEnumerator::sensorName(enums::sensorType::SensorTypeEnum sensor)
{
	return sensorNamesList().at(sensor);
}
