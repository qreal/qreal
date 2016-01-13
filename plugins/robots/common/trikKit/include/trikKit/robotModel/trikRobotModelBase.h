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

#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>
#include <kitBase/robotModel/commonRobotModel.h>

namespace trik {
namespace robotModel {

class TrikRobotModelBase : public kitBase::robotModel::CommonRobotModel
{
	Q_OBJECT

public:
	TrikRobotModelBase(const QString &kitId, const QString &robotId);

	QList<kitBase::robotModel::PortInfo> configurablePorts() const override;
	QList<kitBase::robotModel::DeviceInfo> convertibleBases() const override;

	QHash<QString, int> buttonCodes() const override;

	virtual void setErrorReporter(qReal::ErrorReporterInterface &errorReporter) = 0;

protected:
	virtual kitBase::robotModel::DeviceInfo displayInfo() const;
	virtual kitBase::robotModel::DeviceInfo speakerInfo() const;
	virtual kitBase::robotModel::DeviceInfo buttonInfo() const;

	virtual kitBase::robotModel::DeviceInfo powerMotorInfo() const;
	virtual kitBase::robotModel::DeviceInfo servoMotorInfo() const;
	virtual kitBase::robotModel::DeviceInfo encoderInfo() const;

	virtual kitBase::robotModel::DeviceInfo touchSensorInfo() const;
	virtual kitBase::robotModel::DeviceInfo lightSensorInfo() const;
	virtual kitBase::robotModel::DeviceInfo infraredSensorInfo() const;

	virtual kitBase::robotModel::DeviceInfo sonarSensorInfo() const;

	virtual kitBase::robotModel::DeviceInfo motionSensorInfo() const;

	virtual kitBase::robotModel::DeviceInfo gyroscopeInfo() const;
	virtual kitBase::robotModel::DeviceInfo accelerometerInfo() const;

	virtual kitBase::robotModel::DeviceInfo ledInfo() const;
	virtual kitBase::robotModel::DeviceInfo lineSensorInfo() const;
	virtual kitBase::robotModel::DeviceInfo colorSensorInfo() const;
	virtual kitBase::robotModel::DeviceInfo objectSensorInfo() const;
	virtual kitBase::robotModel::DeviceInfo shellInfo() const;

	virtual kitBase::robotModel::DeviceInfo gamepadButtonInfo() const;
	virtual kitBase::robotModel::DeviceInfo gamepadPadInfo() const;
	virtual kitBase::robotModel::DeviceInfo gamepadPadPressSensorInfo() const;
	virtual kitBase::robotModel::DeviceInfo gamepadWheelInfo() const;
	virtual kitBase::robotModel::DeviceInfo gamepadConnectionIndicatorInfo() const;
};

}
}
