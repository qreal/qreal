#include <QtCore/QStringList>

#include <qrutils/mathUtils/math.h>
#include <qrutils/mathUtils/geometry.h>

#include "constants.h"
#include "sensorsConfiguration.h"

using namespace twoDModel::model;
using namespace interpreterBase::robotModel;

SensorsConfiguration::SensorsConfiguration(const QString &robotModelName)
	: mRobotModelName(robotModelName)
{
}

void SensorsConfiguration::onDeviceConfigurationChanged(const QString &robotModel
		, const PortInfo &port, const DeviceInfo &device, Reason reason)
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

QPointF SensorsConfiguration::position(const PortInfo &port) const
{
	return mSensorsInfo[port].position;
}

void SensorsConfiguration::setPosition(const PortInfo &port, const QPointF &position)
{
	if (!mathUtils::Geometry::eq(mSensorsInfo[port].position, position)) {
		mSensorsInfo[port].position = position;
		emit positionChanged(port);
	}
}

qreal SensorsConfiguration::direction(const PortInfo &port) const
{
	return mSensorsInfo[port].direction;
}

void SensorsConfiguration::setDirection(const PortInfo &port, qreal direction)
{
	if (!mathUtils::Math::eq(mSensorsInfo[port].direction, direction)) {
		mSensorsInfo[port].direction = direction;
		emit rotationChanged(port);
	}
}

DeviceInfo SensorsConfiguration::type(const PortInfo &port) const
{
	return currentConfiguration(mRobotModelName, port);
}

void SensorsConfiguration::serialize(QDomElement &robot, QDomDocument &document) const
{
	QDomElement sensorsElem = document.createElement("sensors");
	robot.appendChild(sensorsElem);

	for (const PortInfo &port: mSensorsInfo.keys()) {
		const DeviceInfo device = currentConfiguration(mRobotModelName, port);
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

		const PortInfo port = PortInfo::fromString(sensorNode.attribute("port"));

		const DeviceInfo type = DeviceInfo::fromString(sensorNode.attribute("type"));

		const QString positionStr = sensorNode.attribute("position", "0:0");
		const QStringList splittedStr = positionStr.split(":");
		qreal const x = static_cast<qreal>(splittedStr[0].toDouble());
		qreal const y = static_cast<qreal>(splittedStr[1].toDouble());
		const QPointF position = QPoint(x, y);

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

SensorsConfiguration::SensorInfo::SensorInfo(const QPointF &position, qreal direction)
	: position(position)
	, direction(direction)
	, isNull(false)
{
}
