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

#include <kitBase/robotModel/commonRobotModel.h>

namespace nxt {
namespace robotModel {

class NxtRobotModelBase : public kitBase::robotModel::CommonRobotModel
{
	Q_OBJECT

public:
	NxtRobotModelBase(const QString &kitId, const QString &robotId);

	QList<kitBase::robotModel::DeviceInfo> convertibleBases() const override;

protected:
	virtual kitBase::robotModel::DeviceInfo displayInfo() const;
	virtual kitBase::robotModel::DeviceInfo speakerInfo() const;
	virtual kitBase::robotModel::DeviceInfo buttonInfo() const;
	virtual kitBase::robotModel::DeviceInfo motorInfo() const;
	virtual kitBase::robotModel::DeviceInfo encoderInfo() const;
	virtual kitBase::robotModel::DeviceInfo touchSensorInfo() const;
	virtual kitBase::robotModel::DeviceInfo sonarSensorInfo() const;
	virtual kitBase::robotModel::DeviceInfo lightSensorInfo() const;
	virtual kitBase::robotModel::DeviceInfo colorFullSensorInfo() const;
	virtual kitBase::robotModel::DeviceInfo colorRedSensorInfo() const;
	virtual kitBase::robotModel::DeviceInfo colorGreenSensorInfo() const;
	virtual kitBase::robotModel::DeviceInfo colorBlueSensorInfo() const;
	virtual kitBase::robotModel::DeviceInfo colorPassiveSensorInfo() const;
	virtual kitBase::robotModel::DeviceInfo soundSensorInfo() const;
	virtual kitBase::robotModel::DeviceInfo gyroscopeSensorInfo() const;
	virtual kitBase::robotModel::DeviceInfo accelerometerSensorInfo() const;
};

}
}
