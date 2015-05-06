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

#include <twoDModel/robotModel/twoDRobotModel.h>

namespace trik {
namespace robotModel {
namespace twoD {

class TwoDRobotModel : public twoDModel::robotModel::TwoDRobotModel
{
	Q_OBJECT

public:
	explicit TwoDRobotModel(kitBase::robotModel::RobotModelInterface &realModel);

	QString robotImage() const override;
	kitBase::robotModel::PortInfo defaultLeftWheelPort() const override;
	kitBase::robotModel::PortInfo defaultRightWheelPort() const override;
	twoDModel::engine::TwoDModelDisplayWidget *displayWidget(QWidget * parent) const override;
	QString sensorImagePath(const kitBase::robotModel::DeviceInfo &deviceType) const override;
	QRect sensorImageRect(const kitBase::robotModel::DeviceInfo &deviceType) const;

	QHash<kitBase::robotModel::PortInfo, kitBase::robotModel::DeviceInfo> specialDevices() const override;
	QPair<QPoint, qreal> specialDeviceConfiguration(const kitBase::robotModel::PortInfo &port) const override;

	void setWheelPorts(const QString &leftWheelPort, const QString &rightWheelPort);

private:
	kitBase::robotModel::robotParts::Device *createDevice(
			const kitBase::robotModel::PortInfo &port
			, const kitBase::robotModel::DeviceInfo &deviceInfo
			) override;

	void onInterpretationStarted() override;

	QString mLeftWheelPort;
	QString mRightWheelPort;
};

}
}
}
