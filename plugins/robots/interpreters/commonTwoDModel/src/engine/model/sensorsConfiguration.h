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
	SensorsConfiguration(const QString &robotModelName);

	void setPosition(const interpreterBase::robotModel::PortInfo &port, const QPointF &position);
	QPointF position(const interpreterBase::robotModel::PortInfo &port) const;

	void setDirection(const interpreterBase::robotModel::PortInfo &port, qreal direction);
	qreal direction(const interpreterBase::robotModel::PortInfo &port) const;

	interpreterBase::robotModel::DeviceInfo type(const interpreterBase::robotModel::PortInfo &port) const;

	void clearSensor(const interpreterBase::robotModel::PortInfo &port);

	void serialize(QDomElement &robot, QDomDocument &document) const;
	void deserialize(const QDomElement &element);

signals:
	/// Emitted when new device is added to configuration.
	/// @param port - port where new device is added.
	/// @param isLoading - true, if we are loading configuration now, so shall not save this device in a diagram.
	void deviceAdded(const interpreterBase::robotModel::PortInfo &port, bool isLoading);

	/// Emitted when a device is removed from configuration.
	/// @param port - port from where device is removed.
	/// @param isLoading - true, if we are loading configuration now, so shall not attempt to save changes in a diagram.
	void deviceRemoved(const interpreterBase::robotModel::PortInfo &port, bool isLoading);

	void positionChanged(const interpreterBase::robotModel::PortInfo &port);
	void rotationChanged(const interpreterBase::robotModel::PortInfo &port);

private:
	struct SensorInfo
	{
		SensorInfo();
		SensorInfo(const QPointF &position, qreal direction);

		QPointF position;
		qreal direction;
		bool isNull;
	};

	void onDeviceConfigurationChanged(const QString &robotModel
			, const interpreterBase::robotModel::PortInfo &port
			, const interpreterBase::robotModel::DeviceInfo &device
			, Reason reason) override;

	QPointF defaultPosition() const;

	QString mRobotModelName;
	QHash<interpreterBase::robotModel::PortInfo, SensorInfo> mSensorsInfo;
};

}
}
