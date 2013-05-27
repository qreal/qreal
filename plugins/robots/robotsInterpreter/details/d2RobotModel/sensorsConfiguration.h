#pragma once

#include <QtCore/QPoint>
#include <QtCore/QVector>
#include <QtXml/QDomDocument>

#include "../../sensorConstants.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace d2Model {

class SensorsConfiguration
{
public:
	SensorsConfiguration();
	void setSensor(inputPort::InputPortEnum const &port
			, sensorType::SensorTypeEnum const &type
			, QPointF const &position
			, qreal const &direction);

	void setPosition(inputPort::InputPortEnum const &port, QPointF const &position);
	QPointF position(inputPort::InputPortEnum const &port) const;

	void setDirection(inputPort::InputPortEnum const &port, qreal direction);
	qreal direction(inputPort::InputPortEnum const &port) const;

	sensorType::SensorTypeEnum type(inputPort::InputPortEnum const &port) const;
	void clearSensor(inputPort::InputPortEnum const &port);

	void serialize(QDomElement &robot, QDomDocument &document) const;
	void deserialize(QDomElement const &element);

private:
	class SensorInfo
	{
	public:
		SensorInfo();
		SensorInfo(QPointF const &position, qreal direction
				, sensorType::SensorTypeEnum const &sensorType);

		QPointF position() const;
		void setPosition(QPointF const &position);

		qreal direction() const;
		void setDirection(qreal direction);

		sensorType::SensorTypeEnum type() const;

	private:
		QPointF mPosition;
		qreal mDirection;
		sensorType::SensorTypeEnum mSensorType;
	};

	QVector<SensorInfo> mSensors;
};

}
}
}
}
}