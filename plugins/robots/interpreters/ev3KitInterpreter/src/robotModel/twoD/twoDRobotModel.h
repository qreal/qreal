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

namespace ev3 {
namespace robotModel {
namespace twoD {

class TwoDRobotModel : public twoDModel::robotModel::TwoDRobotModel
{
	Q_OBJECT

public:
	explicit TwoDRobotModel(kitBase::robotModel::RobotModelInterface const &realModel);

	QString robotImage() const override;
	kitBase::robotModel::PortInfo defaultLeftWheelPort() const override;
	kitBase::robotModel::PortInfo defaultRightWheelPort() const override;
	twoDModel::engine::TwoDModelDisplayWidget *displayWidget() const override;

	QString sensorImagePath(const kitBase::robotModel::DeviceInfo &deviceType) const override;
	QRect sensorImageRect(kitBase::robotModel::DeviceInfo const &deviceType) const;

protected:
	kitBase::robotModel::robotParts::Device *createDevice(
			kitBase::robotModel::PortInfo const &port
			, kitBase::robotModel::DeviceInfo const &deviceInfo
			) override;

private:
	twoDModel::engine::TwoDModelDisplayWidget *mDisplayWidget;  // Does not have ownership
};

}
}
}
