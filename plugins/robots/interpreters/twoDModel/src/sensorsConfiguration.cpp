#include <QtCore/QStringList>

#include "sensorsConfiguration.h"

using namespace twoDModel;

SensorsConfiguration::SensorsConfiguration()
{
	mSensors.resize(4);
}

//void SensorsConfiguration::setSensor(robots::enums::inputPort::InputPortEnum const port
//		, robots::enums::sensorType::SensorTypeEnum const &type
//		, QPointF const &position
//		, qreal const &direction)
//{
//	mSensors[port] = SensorInfo(position, direction, type);
//}

//void SensorsConfiguration::setPosition(robots::enums::inputPort::InputPortEnum const port, QPointF const &position)
//{
//	mSensors[port].setPosition(position);
//}

//QPointF SensorsConfiguration::position(robots::enums::inputPort::InputPortEnum const port) const
//{
//	return mSensors[port].position();
//}

//void SensorsConfiguration::setDirection(robots::enums::inputPort::InputPortEnum const port, qreal direction)
//{
//	mSensors[port].setDirection(direction);
//}

//qreal SensorsConfiguration::direction(robots::enums::inputPort::InputPortEnum const port) const
//{
//	return mSensors[port].direction();
//}

//robots::enums::sensorType::SensorTypeEnum SensorsConfiguration::type(robots::enums::inputPort::InputPortEnum const port) const
//{
//	return mSensors[port].type();
//}

//void SensorsConfiguration::clearSensor(robots::enums::inputPort::InputPortEnum const port)
//{
//	mSensors[port] = SensorInfo();
//}

void SensorsConfiguration::serialize(QDomElement &robot, QDomDocument &document) const
{
	QDomElement sensorsElem = document.createElement("sensors");
	robot.appendChild(sensorsElem);

	int port = 0;
	foreach (SensorInfo const &sensor, mSensors) {
		QDomElement sensorElem = document.createElement("sensor");
		sensorsElem.appendChild(sensorElem);
		sensorElem.setAttribute("port", port);
//		sensorElem.setAttribute("type", sensor.type());
		sensorElem.setAttribute("position", QString::number(sensor.position().x()) + ":" + QString::number(sensor.position().y()));
		sensorElem.setAttribute("direction", sensor.direction());
		++port;
	}
	robot.appendChild(sensorsElem);
}

void SensorsConfiguration::deserialize(QDomElement const &element)
{
	if (element.isNull()) {
		// TODO: Report error
		return;
	}

	mSensors.clear();
	mSensors.resize(4);

	QDomNodeList sensors = element.elementsByTagName("sensor");
	for (int i = 0; i < sensors.count(); ++i) {
		QDomElement const sensorNode = sensors.at(i).toElement();

//		robots::enums::inputPort::InputPortEnum const port
//				= static_cast<robots::enums::inputPort::InputPortEnum>(sensorNode.attribute("port", "0").toInt());

//		robots::enums::sensorType::SensorTypeEnum const type
//				= static_cast<robots::enums::sensorType::SensorTypeEnum>(sensorNode.attribute("type", "0").toInt());

//		QString const positionStr = sensorNode.attribute("position", "0:0");
//		QStringList const splittedStr = positionStr.split(":");
//		qreal const x = static_cast<qreal>(splittedStr[0].toDouble());
//		qreal const y = static_cast<qreal>(splittedStr[1].toDouble());
//		QPointF const position = QPoint(x, y);

//		qreal const direction = sensorNode.attribute("direction", "0").toDouble();

//		setSensor(port, type, position, direction);
	}
}


SensorsConfiguration::SensorInfo::SensorInfo()
	: mDirection(0)  //, mSensorType(robots::enums::sensorType::unused)
{
}

//SensorsConfiguration::SensorInfo::SensorInfo(QPointF const &position
//		, qreal direction
//		, robots::enums::sensorType::SensorTypeEnum const &sensorType)
//	: mPosition(position)
//	, mDirection(direction)
//	, mSensorType(sensorType)
//{
//}

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

//robots::enums::sensorType::SensorTypeEnum SensorsConfiguration::SensorInfo::type() const
//{
//	return mSensorType;
//}
