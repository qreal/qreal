/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#pragma once

#include <QtCore/QPoint>
#include <QtCore/QHash>
#include <QtXml/QDomDocument>

#include <kitBase/devicesConfigurationProvider.h>

#include "twoDModel/twoDModelDeclSpec.h"

namespace twoDModel {
namespace model {

/// Represents position and direction of simulated sensors relative to robot.
class TWO_D_MODEL_EXPORT SensorsConfiguration : public QObject, public kitBase::DevicesConfigurationProvider
{
	Q_OBJECT

public:
	SensorsConfiguration(const QString &robotModelName);

	void setPosition(const kitBase::robotModel::PortInfo &port, const QPointF &position);
	QPointF position(const kitBase::robotModel::PortInfo &port) const;

	void setDirection(const kitBase::robotModel::PortInfo &port, qreal direction);
	qreal direction(const kitBase::robotModel::PortInfo &port) const;

	kitBase::robotModel::DeviceInfo type(const kitBase::robotModel::PortInfo &port) const;

	void clearSensor(const kitBase::robotModel::PortInfo &port);

	void serialize(QDomElement &robot, QDomDocument &document) const;
	void deserialize(const QDomElement &element);

signals:
	/// Emitted when new device is added to configuration.
	/// @param port - port where new device is added.
	/// @param isLoading - true, if we are loading configuration now, so shall not save this device in a diagram.
	void deviceAdded(const kitBase::robotModel::PortInfo &port, bool isLoading);

	/// Emitted when a device is removed from configuration.
	/// @param port - port from where device is removed.
	/// @param isLoading - true, if we are loading configuration now, so shall not attempt to save changes in a diagram.
	void deviceRemoved(const kitBase::robotModel::PortInfo &port, bool isLoading);

	void positionChanged(const kitBase::robotModel::PortInfo &port);
	void rotationChanged(const kitBase::robotModel::PortInfo &port);

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
			, const kitBase::robotModel::PortInfo &port
			, const kitBase::robotModel::DeviceInfo &device
			, Reason reason) override;

	QPointF defaultPosition() const;

	QString mRobotModelName;
	QHash<kitBase::robotModel::PortInfo, SensorInfo> mSensorsInfo;
};

}
}
