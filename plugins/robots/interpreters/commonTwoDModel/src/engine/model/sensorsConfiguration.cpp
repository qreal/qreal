#include <QtCore/QStringList>

#include "sensorsConfiguration.h"

using namespace twoDModel::model;
using namespace interpreterBase::robotModel;

SensorsConfiguration::SensorsConfiguration(QString const &robotModelName)
	: mRobotModel(robotModelName)
{
}

void SensorsConfiguration::onDeviceConfigurationChanged(QString const &robotModel
		, PortInfo const &port, DeviceInfo const &device)
{
	if (robotModel != mRobotModel) {
		// Ignoring external events
		return;
	}

	if (device.isNull()) {
		mSensorsInfo[port] = SensorInfo();
		emit deviceRemoved(port);
		return;
	}

	emit deviceAdded(port);
}

void SensorsConfiguration::setPosition(PortInfo const &port, QPointF const &position)
{
	mSensorsInfo[port].position = position;
}

QPointF SensorsConfiguration::position(PortInfo const &port) const
{
	return mSensorsInfo[port].position;
}

void SensorsConfiguration::setDirection(PortInfo const &port, qreal direction)
{
	mSensorsInfo[port].direction = direction;
}

qreal SensorsConfiguration::direction(PortInfo const &port) const
{
	return mSensorsInfo[port].direction;
}

DeviceInfo SensorsConfiguration::type(PortInfo const &port) const
{
	return currentConfiguration(mRobotModel, port);
}

void SensorsConfiguration::serialize(QDomElement &robot, QDomDocument &document) const
{
	QDomElement sensorsElem = document.createElement("sensors");
	robot.appendChild(sensorsElem);

	for (PortInfo const &port: mSensorsInfo.keys()) {
		DeviceInfo const device = currentConfiguration(mRobotModel, port);
		SensorInfo const sensor = mSensorsInfo.value(port);
		QDomElement sensorElem = document.createElement("sensor");
		sensorsElem.appendChild(sensorElem);
		sensorElem.setAttribute("port", port.toString());
		sensorElem.setAttribute("type", device.toString());

		sensorElem.setAttribute("position"
				, QString::number(sensor.position.x()) + ":" + QString::number(sensor.position.y()));

		sensorElem.setAttribute("direction", sensor.direction);
	}

	robot.appendChild(sensorsElem);
}

void SensorsConfiguration::deserialize(QDomElement const &element)
{
	if (element.isNull()) {
		/// @todo Report error
		return;
	}

	mSensorsInfo.clear();

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

		deviceConfigurationChanged(mRobotModel, port, type);
		setPosition(port, position);
		setDirection(port, direction);
	}
}

SensorsConfiguration::SensorInfo::SensorInfo()
	: direction(0)
{
}

SensorsConfiguration::SensorInfo::SensorInfo(QPointF const &position, qreal direction)
	: position(position)
	, direction(direction)
{
}
