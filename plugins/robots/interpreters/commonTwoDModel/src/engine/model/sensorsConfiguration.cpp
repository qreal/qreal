#include <QtCore/QStringList>

#include "sensorsConfiguration.h"

using namespace twoDModel;
using namespace interpreterBase::robotModel;

SensorsConfiguration::SensorsConfiguration()
{
}

void SensorsConfiguration::setSensor(PortInfo const &port
		, DeviceInfo const &type
		, QPointF const &position
		, qreal const &direction)
{
	mSensors[port] = SensorInfo(position, direction, type);
}

void SensorsConfiguration::setPosition(PortInfo const &port, QPointF const &position)
{
	mSensors[port].setPosition(position);
}

QPointF SensorsConfiguration::position(PortInfo const &port) const
{
	return mSensors[port].position();
}

void SensorsConfiguration::setDirection(PortInfo const &port, qreal direction)
{
	mSensors[port].setDirection(direction);
}

qreal SensorsConfiguration::direction(PortInfo const &port) const
{
	return mSensors[port].direction();
}

DeviceInfo SensorsConfiguration::type(PortInfo const &port) const
{
	return mSensors[port].type();
}

void SensorsConfiguration::clearSensor(PortInfo const &port)
{
	mSensors[port] = SensorInfo();
}

void SensorsConfiguration::serialize(QDomElement &robot, QDomDocument &document) const
{
	QDomElement sensorsElem = document.createElement("sensors");
	robot.appendChild(sensorsElem);

	for (PortInfo const &port: mSensors.keys()) {
		SensorInfo const &sensor = mSensors.value(port);
		QDomElement sensorElem = document.createElement("sensor");
		sensorsElem.appendChild(sensorElem);
		sensorElem.setAttribute("port", port.toString());
		sensorElem.setAttribute("type", sensor.type().toString());

		sensorElem.setAttribute("position"
				, QString::number(sensor.position().x()) + ":" + QString::number(sensor.position().y()));

		sensorElem.setAttribute("direction", sensor.direction());
	}

	robot.appendChild(sensorsElem);
}

void SensorsConfiguration::deserialize(QDomElement const &element)
{
	if (element.isNull()) {
		/// @todo Report error
		return;
	}

	mSensors.clear();

	QDomNodeList sensors = element.elementsByTagName("sensor");
	for (int i = 0; i < sensors.count(); ++i) {
		QDomElement const sensorNode = sensors.at(i).toElement();

		PortInfo const port = PortInfo::fromString(sensorNode.attribute("port"));

		DeviceInfo const type = DeviceInfo::fromString(sensorNode.attribute("type"));

		QString const positionStr = sensorNode.attribute("position", "0:0");
		QStringList const splittedStr = positionStr.split(":");
		qreal const x = static_cast<qreal>(splittedStr[0].toDouble());
		qreal const y = static_cast<qreal>(splittedStr[1].toDouble());
		QPointF const position = QPoint(x, y);

		qreal const direction = sensorNode.attribute("direction", "0").toDouble();

		setSensor(port, type, position, direction);
	}
}


SensorsConfiguration::SensorInfo::SensorInfo()
	: mDirection(0)
{
}

SensorsConfiguration::SensorInfo::SensorInfo(QPointF const &position
		, qreal direction
		, DeviceInfo const &sensorType)
	: mPosition(position)
	, mDirection(direction)
	, mSensorType(sensorType)
{
}

QPointF SensorsConfiguration::SensorInfo::position() const
{
	return mPosition;
}

void SensorsConfiguration::SensorInfo::setPosition(QPointF const &position)
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

DeviceInfo const &SensorsConfiguration::SensorInfo::type() const
{
	return mSensorType;
}
