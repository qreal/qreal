#include <QtCore/QStringList>

#include "sensorsConfiguration.h"

using namespace qReal::interpreters::robots;
using namespace details::d2Model;

SensorsConfiguration::SensorsConfiguration()
{
	mSensors.resize(4);
}

void SensorsConfiguration::setSensor(inputPort::InputPortEnum const &port
		, sensorType::SensorTypeEnum const &type
		, QPointF const &position
		, qreal const &direction
		, bool const sticked)
{
	mSensors[port] = SensorInfo(position, direction, type, sticked);
}

void SensorsConfiguration::setPosition(inputPort::InputPortEnum const &port, QPointF const &position)
{
	mSensors[port].setPosition(position);
}

QPointF SensorsConfiguration::position(inputPort::InputPortEnum const &port) const
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

bool SensorsConfiguration::stickedToItem(inputPort::InputPortEnum const port) const
{
	return mSensors[port].stickedToRobot();
}

void SensorsConfiguration::onStickedToItem(inputPort::InputPortEnum const port, bool sticked)
{
	// In current logic sensors can be sticked to the only item (robot)
	mSensors[port].setStickedToRobot(sticked);
}

void SensorsConfiguration::serialize(QDomElement &robot, QDomDocument &document) const
{
	QDomElement sensorsElem = document.createElement("sensors");
	robot.appendChild(sensorsElem);

	int port = 0;
	foreach (SensorInfo const &sensor, mSensors) {
		QDomElement sensorElem = document.createElement("sensor");
		sensorsElem.appendChild(sensorElem);
		sensorElem.setAttribute("port", port);
		sensorElem.setAttribute("type", sensor.type());
		sensorElem.setAttribute("position", QString::number(sensor.position().x()) + ":" + QString::number(sensor.position().y()));
		sensorElem.setAttribute("direction", sensor.direction());
		sensorElem.setAttribute("sticked", sensor.stickedToRobot());
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

		inputPort::InputPortEnum const port = static_cast<inputPort::InputPortEnum>(sensorNode.attribute("port", "0").toInt());
		sensorType::SensorTypeEnum const type = static_cast<sensorType::SensorTypeEnum>(sensorNode.attribute("type", "0").toInt());

		QString const positionStr = sensorNode.attribute("position", "0:0");
		QStringList const splittedStr = positionStr.split(":");
		qreal const x = static_cast<qreal>(splittedStr[0].toDouble());
		qreal const y = static_cast<qreal>(splittedStr[1].toDouble());
		QPointF const position = QPoint(x, y);

		qreal const direction = sensorNode.attribute("direction", "0").toDouble();
		bool const sticked = sensorNode.attribute("sticked", "1").toInt() != 0;

		setSensor(port, type, position, direction, sticked);
	}
}


SensorsConfiguration::SensorInfo::SensorInfo()
	: mDirection(0), mSensorType(sensorType::unused)
{
}

SensorsConfiguration::SensorInfo::SensorInfo(QPointF const &position
		, qreal direction
		, sensorType::SensorTypeEnum const &sensorType
		, bool const sticked)
	: mPosition(position), mDirection(direction)
	, mSensorType(sensorType), mStickedToRobot(sticked)
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

bool SensorsConfiguration::SensorInfo::stickedToRobot() const
{
	return mStickedToRobot;
}

void SensorsConfiguration::SensorInfo::setStickedToRobot(bool sticked)
{
	mStickedToRobot = sticked;
}

void SensorsConfiguration::SensorInfo::setDirection(qreal direction)
{
	mDirection = direction;
}

sensorType::SensorTypeEnum SensorsConfiguration::SensorInfo::type() const
{
	return mSensorType;
}
