/* Copyright 2013-2016 CyberTech Labs Ltd.
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

#include <QtCore/QString>

#include <kitBase/robotModel/commonRobotModel.h>

#include "twoDModel/twoDModelDeclSpec.h"
#include "twoDModel/engine/twoDModelDisplayWidget.h"

namespace twoDModel {

namespace engine {
class TwoDModelEngineInterface;
}

namespace robotModel {

/// A robot model implementation common for all 2D model wrappers.
/// Accepts the given real robot model implementation proxying its devices, ports and convertible bases.
class TWO_D_MODEL_EXPORT TwoDRobotModel : public kitBase::robotModel::CommonRobotModel
{
	Q_OBJECT

public:
	explicit TwoDRobotModel(const RobotModelInterface &realModel);
	explicit TwoDRobotModel(const QString &robotId);

	QString name() const override;
	QString friendlyName() const override;
	bool needsConnection() const override;
	int updateIntervalForInterpretation() const override;
	utils::TimelineInterface &timeline() override;
	QList<kitBase::robotModel::PortInfo> configurablePorts() const override;
	QList<kitBase::robotModel::DeviceInfo> convertibleBases() const override;
	int priority() const override;

	/// Configures 2D model window`s engine for using it in 2D model devices emulators.
	void setEngine(engine::TwoDModelEngineInterface &engine);

	/// @todo move physical constants here

	/// Returns a path to an image that is used for robot item.
	virtual QString robotImage() const = 0;

	/// Returns a port that is used for left robot wheel by default.
	virtual kitBase::robotModel::PortInfo defaultLeftWheelPort() const = 0;

	/// Returns a port that is used for right robot wheel by default.
	virtual kitBase::robotModel::PortInfo defaultRightWheelPort() const = 0;

	/// Returns a pointer to a widget with display emulator for current robot model.
	virtual engine::TwoDModelDisplayWidget *displayWidget() const = 0;

	/// Returns robot`s polygon. By default returns polygon constructed as rectangle using size.
	virtual QPolygonF collidingPolygon() const;

	/// Returns robot`s mass in kilograms. Mass will be used by physical engine to process collisions correctly.
	virtual qreal mass() const = 0;

	/// Returns robot`s fixture friction coefficient.
	virtual qreal friction() const = 0;

	/// Returns robot`s rectangle size in pixels.
	/// By default returns (50, 50).
	virtual QSizeF size() const;

	/// Returns a point (pixels) in items coordinates arround which robot rotates.
	/// By default returns center of robot`s rectangle.
	virtual QPointF rotationCenter() const;

	/// Returns a point(pixels) list of wheels coordinates on robot.
	virtual QList<QPointF> wheelsPosition() const = 0;

	/// Returns an average number of degrees that motor rotates per one second on power 1.
	virtual qreal onePercentAngularVelocity() const = 0;

	/// Provides path to sensor image by given device type or empty string if default sensor image shall be used.
	virtual QString sensorImagePath(const kitBase::robotModel::DeviceInfo &deviceType) const
	{
		Q_UNUSED(deviceType)

		return "";
	}

	/// Provides sensor image bounding rectangle by given device type or empty rectangle if default image size be used.
	virtual QRect sensorImageRect(const kitBase::robotModel::DeviceInfo &deviceType) const
	{
		Q_UNUSED(deviceType)

		return QRect();
	}

	/// Returns ports and info of devices that are specific to a particular robot model and non-configurable.
	virtual QHash<kitBase::robotModel::PortInfo, kitBase::robotModel::DeviceInfo> specialDevices() const
	{
		return QHash<kitBase::robotModel::PortInfo, kitBase::robotModel::DeviceInfo>();
	}

	/// Returns position and direction of a special device image relative to robot image center
	/// (for example, -1 stands for left or top side, 1 stands for right or bottom side of an image).
	virtual QPair<QPoint, qreal> specialDeviceConfiguration(const kitBase::robotModel::PortInfo &port) const
	{
		Q_UNUSED(port)

		return qMakePair(QPoint(), 0);
	}

	/// Returns a reference to object providing different parts of 2D model emulator.
	engine::TwoDModelEngineInterface *engine();

protected:
	kitBase::robotModel::robotParts::Device *createDevice(
			const kitBase::robotModel::PortInfo &port
			, const kitBase::robotModel::DeviceInfo &deviceInfo
			) override;

	virtual kitBase::robotModel::DeviceInfo markerInfo() const;

private:
	const kitBase::robotModel::RobotModelInterface *mRealModel;
	engine::TwoDModelEngineInterface *mEngine = nullptr;  // Does not have ownership.
};

}
}
