#include "sensorsConfiguration.h"

#include <QtCore/QStringList>

using namespace qReal::interpreters::robots;
using namespace details::d2Model;

SensorsConfiguration::SensorsConfiguration()
{
	mSensors.resize(4);
}

void SensorsConfiguration::setSensor(inputPort::InputPortEnum const &port
		, sensorType::SensorTypeEnum const &type
		, QPoint const &position
		, qreal const &direction)
{
	mSensors[port] = SensorInfo(position, direction, type);
}

void SensorsConfiguration::setPosition(inputPort::InputPortEnum const &port, QPoint const &position)
{
	mSensors[port].setPosition(position);
}

QPoint SensorsConfiguration::position(inputPort::InputPortEnum const &port) const
{
	return mSensors[port].position();
}

void SensorsConfiguration::setDirection(inputPort::InputPortEnum const &port, qreal direction)
{
	mSensors[port].setDirection(direction);
}

qreal SensorsConfiguration::direction(inputPort::InputPortEnum const &port) const
{
	return mSensors[port].direction();
}

sensorType::SensorTypeEnum SensorsConfiguration::type(inputPort::InputPortEnum const &port) const
{
	return mSensors[port].type();
}

void SensorsConfiguration::clearSensor(inputPort::InputPortEnum const &port)
{
	mSensors[port] = SensorInfo();
}

QDomDocument SensorsConfiguration::serialize() const
{
	QDomDocument result;

	int port = 0;
	foreach (SensorInfo const &sensor, mSensors) {
		QDomElement sensorElem = result.createElement("sensor");
		result.appendChild(sensorElem);
		sensorElem.setAttribute("port", port);
		sensorElem.setAttribute("type", sensor.type());
		sensorElem.setAttribute("position", QString::number(sensor.position().x()) + ":" + QString::number(sensor.position().y()));
		sensorElem.setAttribute("direction", sensor.direction());
		++port;
	}

	return result;
}

void SensorsConfiguration::deserialize(QDomDocument const &document)
{
	mSensors.clear();
	mSensors.resize(4);

	QDomNodeList sensors = document.elementsByTagName("sensor");
	for (int i = 0; i < sensors.count(); ++i) {
		QDomElement const sensorNode = sensors.at(i).toElement();

		inputPort::InputPortEnum const port = static_cast<inputPort::InputPortEnum>(sensorNode.attribute("port", "0").toInt());

		sensorType::SensorTypeEnum const type = static_cast<sensorType::SensorTypeEnum>(sensorNode.attribute("type", "0").toInt());

		QString const positionStr = sensorNode.attribute("position", "0:0");
		QStringList const splittedStr = positionStr.split(":");
		int const x = splittedStr[0].toInt();
		int const y = splittedStr[1].toInt();
		QPoint const position = QPoint(x, y);

		qreal const direction = sensorNode.attribute("direction", "0").toDouble();

		setSensor(port, type, position, direction);
	}
}


SensorsConfiguration::SensorInfo::SensorInfo()
	: mDirection(0), mSensorType(sensorType::unused)
{
}

SensorsConfiguration::SensorInfo::SensorInfo(QPoint const &position
		, qreal direction
		, sensorType::SensorTypeEnum const &sensorType
)
	: mPosition(position), mDirection(direction), mSensorType(sensorType)
{
}

QPoint SensorsConfiguration::SensorInfo::position() const
{
	return mPosition;
}

void SensorsConfiguration::SensorInfo::setPosition(QPoint const &position)
{
	mPosition = position;
}

qreal SensorsConfiguration::SensorInfo::direction() const
{
	return mDirection;
}

void SensorsConfiguration::SensorInfo::setDirection(qreal direction)
{
	mDirection = direction;
}

sensorType::SensorTypeEnum SensorsConfiguration::SensorInfo::type() const
{
	return mSensorType;
}
