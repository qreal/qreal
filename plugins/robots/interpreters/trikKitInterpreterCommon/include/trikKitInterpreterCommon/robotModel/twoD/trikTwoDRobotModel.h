/* Copyright 2015 CyberTech Labs Ltd.
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

#include "trikKitInterpreterCommon/declSpec.h"

namespace qReal {
class ErrorReporterInterface;
}

namespace trik {
namespace robotModel {
namespace twoD {

/// @todo Refactor common code out of here.
///       It needs concerns separation: robot model is device factory and container for device-port configuration.
///       So, for different TRIK cases devices are the same, but mappings between ports and devices are
///       different. And across models devices are different (2d and real) but port mappings are the same.
///       Clearly we need to separate this into two hierarchies.
class ROBOTS_TRIK_KIT_INTERPRETER_COMMON_EXPORT TrikTwoDRobotModel : public twoDModel::robotModel::TwoDRobotModel
{
	Q_OBJECT

public:
	explicit TrikTwoDRobotModel(kitBase::robotModel::RobotModelInterface &realModel);

	QString robotImage() const override;

	kitBase::robotModel::PortInfo defaultLeftWheelPort() const override;

	kitBase::robotModel::PortInfo defaultRightWheelPort() const override;

	twoDModel::engine::TwoDModelDisplayWidget *displayWidget() const override;

	QString sensorImagePath(const kitBase::robotModel::DeviceInfo &deviceType) const override;

	QRect sensorImageRect(const kitBase::robotModel::DeviceInfo &deviceType) const;

	QHash<kitBase::robotModel::PortInfo, kitBase::robotModel::DeviceInfo> specialDevices() const override;

	QPair<QPoint, qreal> specialDeviceConfiguration(const kitBase::robotModel::PortInfo &port) const override;

	void setWheelPorts(const QString &leftWheelPort, const QString &rightWheelPort);

	QHash<QString, int> buttonCodes() const override;

	/// Sets the error reporter for writing bubbling messages by shell emulator.
	void setErrorReporter(qReal::ErrorReporterInterface &errorReporter);

private:
	kitBase::robotModel::robotParts::Device *createDevice(
			const kitBase::robotModel::PortInfo &port
			, const kitBase::robotModel::DeviceInfo &deviceInfo
			) override;

	void onInterpretationStarted() override;

	QString mLeftWheelPort;
	QString mRightWheelPort;
	twoDModel::engine::TwoDModelDisplayWidget *mDisplayWidget;
	qReal::ErrorReporterInterface *mErrorReporter;
};

}
}
}
