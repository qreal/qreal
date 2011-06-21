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
			, QPoint const &position
			, qreal const &direction);

	void setPosition(inputPort::InputPortEnum const &port, QPoint const &position);
	QPoint position(inputPort::InputPortEnum const &port) const;

	void setDirection(inputPort::InputPortEnum const &port, qreal direction);
	qreal direction(inputPort::InputPortEnum const &port) const;

	sensorType::SensorTypeEnum type(inputPort::InputPortEnum const &port) const;
	void clearSensor(inputPort::InputPortEnum const &port);

	QDomElement  serialize(QDomDocument &document) const;
	void deserialize(QDomElement const &element);

private:
	class SensorInfo
	{
	public:
		SensorInfo();
		SensorInfo(QPoint const &position, qreal direction, sensorType::SensorTypeEnum const &sensorType);

		QPoint position() const;
		void setPosition(QPoint const &position);

		qreal direction() const;
		void setDirection(qreal direction);

		sensorType::SensorTypeEnum type() const;

	private:
		QPoint mPosition;
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
