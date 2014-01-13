#pragma once

#include <QtCore/QPoint>
#include <QtCore/QVector>
#include <QtXml/QDomDocument>

//#include "../../sensorConstants.h"

namespace twoDModel {

class SensorsConfiguration
{
public:
	SensorsConfiguration();
//	void setSensor(robots::enums::inputPort::InputPortEnum const port
//			, robots::enums::sensorType::SensorTypeEnum const &type
//			, QPointF const &position
//			, qreal const &direction);

//	void setPosition(robots::enums::inputPort::InputPortEnum const port, QPointF const &position);
//	QPointF position(robots::enums::inputPort::InputPortEnum const port) const;

//	void setDirection(robots::enums::inputPort::InputPortEnum const port, qreal direction);
//	qreal direction(robots::enums::inputPort::InputPortEnum const port) const;

//	robots::enums::sensorType::SensorTypeEnum type(robots::enums::inputPort::InputPortEnum const port) const;
//	void clearSensor(robots::enums::inputPort::InputPortEnum const port);

	void serialize(QDomElement &robot, QDomDocument &document) const;
	void deserialize(QDomElement const &element);

private:
	class SensorInfo
	{
	public:
		SensorInfo();
//		SensorInfo(QPointF const &position, qreal direction
//				, robots::enums::sensorType::SensorTypeEnum const &sensorType);

		QPointF position() const;
		void setPosition(QPointF const &position);

		qreal direction() const;
		void setDirection(qreal direction);

//		robots::enums::sensorType::SensorTypeEnum type() const;

	private:
		QPointF mPosition;
		qreal mDirection;
//		robots::enums::sensorType::SensorTypeEnum mSensorType;
	};

	QVector<SensorInfo> mSensors;
};

}
