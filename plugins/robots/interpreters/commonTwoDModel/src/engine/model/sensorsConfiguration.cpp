#include <QtCore/QStringList>

#include <qrutils/mathUtils/math.h>
#include <qrutils/mathUtils/geometry.h>

#include "constants.h"
#include "sensorsConfiguration.h"

using namespace twoDModel::model;
using namespace interpreterBase::robotModel;

SensorsConfiguration::SensorsConfiguration(QString const &robotModelName)
	: mRobotModelName(robotModelName)
{
}

void SensorsConfiguration::onDeviceConfigurationChanged(QString const &robotModel
		, PortInfo const &port, DeviceInfo const &device, Reason reason)
{
	if (robotModel != mRobotModelName) {
		// Ignoring external events
		return;
	}

	if (device.isNull()) {
		mSensorsInfo[port] = SensorInfo();
		emit deviceRemoved(port, reason == Reason::loading);
		return;
	}

	emit deviceAdded(port, reason == Reason::loading);

	// If there was no sensor before then placing it right in front of the robot;
	// else putting it instead of old one.
	mSensorsInfo[port] = mSensorsInfo[port].isNull ? SensorInfo(defaultPosition(), 0) : mSensorsInfo[port];
}

QPointF SensorsConfiguration::defaultPosition() const
{
	/// @todo: Move it somewhere?
	return QPointF(robotWidth * 3 / 2, robotHeight / 2);
}

QPointF SensorsConfiguration::position(PortInfo const &port) const
{
	return mSensorsInfo[port].position;
}

void SensorsConfiguration::setPosition(PortInfo const &port, QPointF const &position)
{
	if (!mathUtils::Geometry::eq(mSensorsInfo[port].position, position)) {
		mSensorsInfo[port].position = position;
		emit positionChanged(port);
	}
}

qreal SensorsConfiguration::direction(PortInfo const &port) const
{
	return mSensorsInfo[port].direction;
}

void SensorsConfiguration::setDirection(PortInfo const &port, qreal direction)
{
	if (!mathUtils::Math::eq(mSensorsInfo[port].direction, direction)) {
		mSensorsInfo[port].direction = direction;
		emit rotationChanged(port);
	}
}

DeviceInfo SensorsConfiguration::type(PortInfo const &port) const
{
	return currentConfiguration(mRobotModelName, port);
}

void SensorsConfiguration::serialize(QDomElement &robot, QDomDocument &document) const
{
	QDomElement sensorsElem = document.createElement("sensors");
	robot.appendChild(sensorsElem);

	for (PortInfo const &port: mSensorsInfo.keys()) {
		DeviceInfo const device = currentConfiguration(mRobotModelName, port);
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

		deviceConfigurationChanged(mRobotModelName, port, DeviceInfo(), Reason::loading);
		deviceConfigurationChanged(mRobotModelName, port, type, Reason::loading);
		setPosition(port, position);
		setDirection(port, direction);
	}
}

SensorsConfiguration::SensorInfo::SensorInfo()
	: direction(0)
	, isNull(true)
{
}

SensorsConfiguration::SensorInfo::SensorInfo(QPointF const &position, qreal direction)
	: position(position)
	, direction(direction)
	, isNull(false)
{
}
