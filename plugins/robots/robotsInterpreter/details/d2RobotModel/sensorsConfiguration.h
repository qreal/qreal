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
			, qreal const &direction, const bool sticked);

	void setPosition(inputPort::InputPortEnum const &port, QPoint const &position);
	QPoint position(inputPort::InputPortEnum const &port) const;

	void setDirection(inputPort::InputPortEnum const &port, qreal direction);
	qreal direction(inputPort::InputPortEnum const &port) const;

	sensorType::SensorTypeEnum type(inputPort::InputPortEnum const &port) const;
	void clearSensor(inputPort::InputPortEnum const &port);

	void serialize(QDomElement &robot, QDomDocument &document) const;
	void deserialize(QDomElement const &element);

	bool stickedToItem(inputPort::InputPortEnum const port) const;
	void onStickedToItem(inputPort::InputPortEnum const port, bool sticked);

private:
	class SensorInfo
	{
	public:
		SensorInfo();
		SensorInfo(QPoint const &position, qreal direction
				, sensorType::SensorTypeEnum const &sensorType
				, bool const sticked);

		QPoint position() const;
		void setPosition(QPoint const &position);

		qreal direction() const;
		void setDirection(qreal direction);

		bool stickedToRobot() const;
		void setStickedToRobot(bool sticked);

		sensorType::SensorTypeEnum type() const;

	private:
		QPoint mPosition;
		qreal mDirection;
		sensorType::SensorTypeEnum mSensorType;
		bool mStickedToRobot;
	};

	QVector<SensorInfo> mSensors;
};

}
}
}
}
}
