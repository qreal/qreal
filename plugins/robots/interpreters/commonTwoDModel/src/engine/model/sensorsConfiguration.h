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
	explicit SensorsConfiguration(QString const &robotModelName);

	void setPosition(interpreterBase::robotModel::PortInfo const &port, QPointF const &position);
	QPointF position(interpreterBase::robotModel::PortInfo const &port) const;

	void setDirection(interpreterBase::robotModel::PortInfo const &port, qreal direction);
	qreal direction(interpreterBase::robotModel::PortInfo const &port) const;

	interpreterBase::robotModel::DeviceInfo type(interpreterBase::robotModel::PortInfo const &port) const;

	void clearSensor(interpreterBase::robotModel::PortInfo const &port);

	void serialize(QDomElement &robot, QDomDocument &document) const;
	void deserialize(QDomElement const &element);

signals:
	/// Emitted when new device is added to configuration.
	/// @param port - port where new device is added.
	/// @param isLoading - true, if we are loading configuration now, so shall not save this device in a diagram.
	void deviceAdded(interpreterBase::robotModel::PortInfo const &port, bool isLoading);

	/// Emitted when a device is removed from configuration.
	/// @param port - port from where device is removed.
	/// @param isLoading - true, if we are loading configuration now, so shall not attempt to save changes in a diagram.
	void deviceRemoved(interpreterBase::robotModel::PortInfo const &port, bool isLoading);

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
			, interpreterBase::robotModel::DeviceInfo const &device
			, Reason reason) override;

	QPointF defaultPosition() const;

	QString const mRobotModel;
	QHash<interpreterBase::robotModel::PortInfo, SensorInfo> mSensorsInfo;
};

}
}
