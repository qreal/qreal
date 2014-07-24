#pragma once

#include <QtCore/QPoint>
#include <QtCore/QHash>
#include <QtXml/QDomDocument>

#include <interpreterBase/devicesConfigurationProvider.h>

namespace twoDModel {
namespace model {

/// Represents position and direction of simulated sensors relative to robot.
class SensorsConfiguration : public QObject, public interpreterBase::DevicesConfigurationProvider
{
	Q_OBJECT

public:
	SensorsConfiguration(QString const &robotModelName);

	void setPosition(interpreterBase::robotModel::PortInfo const &port, QPointF const &position);
	QPointF position(interpreterBase::robotModel::PortInfo const &port) const;

	void setDirection(interpreterBase::robotModel::PortInfo const &port, qreal direction);
	qreal direction(interpreterBase::robotModel::PortInfo const &port) const;

	interpreterBase::robotModel::DeviceInfo type(interpreterBase::robotModel::PortInfo const &port) const;

	void clearSensor(interpreterBase::robotModel::PortInfo const &port);

	void serialize(QDomElement &robot, QDomDocument &document) const;
	void deserialize(QDomElement const &element);

signals:
	void deviceAdded(interpreterBase::robotModel::PortInfo const &port);
	void deviceRemoved(interpreterBase::robotModel::PortInfo const &port);

	void positionChanged(interpreterBase::robotModel::PortInfo const &port);
	void rotationChanged(interpreterBase::robotModel::PortInfo const &port);

private:
	struct SensorInfo
	{
		SensorInfo();
		SensorInfo(QPointF const &position, qreal direction);

		QPointF position;
		qreal direction;
		bool isNull;
	};

	void onDeviceConfigurationChanged(QString const &robotModel
			, interpreterBase::robotModel::PortInfo const &port
			, interpreterBase::robotModel::DeviceInfo const &device) override;

	QPointF defaultPosition() const;

	QString mRobotModelName;
	QHash<interpreterBase::robotModel::PortInfo, SensorInfo> mSensorsInfo;
};

}
}
