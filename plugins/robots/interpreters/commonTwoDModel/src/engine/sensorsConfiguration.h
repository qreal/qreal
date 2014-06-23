#pragma once

#include <QtCore/QPoint>
#include <QtCore/QHash>
#include <QtXml/QDomDocument>

#include <interpreterBase/robotModel/portInfo.h>
#include <interpreterBase/robotModel/deviceInfo.h>

namespace twoDModel {

/// Represents position and direction of simulated sensors relative to robot.
class SensorsConfiguration
{
public:
	SensorsConfiguration();

	void setSensor(interpreterBase::robotModel::PortInfo const &port
			, interpreterBase::robotModel::DeviceInfo const &type
			, QPointF const &position
			, qreal const &direction);

	void setPosition(interpreterBase::robotModel::PortInfo const &port, QPointF const &position);
	QPointF position(interpreterBase::robotModel::PortInfo const &port) const;

	void setDirection(interpreterBase::robotModel::PortInfo const &port, qreal direction);
	qreal direction(interpreterBase::robotModel::PortInfo const &port) const;

	interpreterBase::robotModel::DeviceInfo type(interpreterBase::robotModel::PortInfo const &port) const;

	void clearSensor(interpreterBase::robotModel::PortInfo const &port);

	void serialize(QDomElement &robot, QDomDocument &document) const;
	void deserialize(QDomElement const &element);

private:
	class SensorInfo
	{
	public:
		SensorInfo();
		SensorInfo(QPointF const &position, qreal direction
				, interpreterBase::robotModel::DeviceInfo const &sensorType);

		QPointF position() const;
		void setPosition(QPointF const &position);

		qreal direction() const;
		void setDirection(qreal direction);

		interpreterBase::robotModel::DeviceInfo const &type() const;

	private:
		QPointF mPosition;
		qreal mDirection;
		interpreterBase::robotModel::DeviceInfo mSensorType;
	};

	QHash<interpreterBase::robotModel::PortInfo, SensorInfo> mSensors;
};

}
