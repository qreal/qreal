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

#include <trikKit/robotModel/trikRobotModelBase.h>

namespace trik {
namespace robotModel {

class TrikGeneratorRobotModel : public TrikRobotModelBase
{
	Q_OBJECT

public:
	TrikGeneratorRobotModel(const QString &kitId, const QString &robotId
			, const QString &name, const QString &friendlyName, int priority);

	QString name() const override;
	QString friendlyName() const override;

	bool needsConnection() const override;
	bool interpretedModel() const override;

	int priority() const override;

	/// Adds pre-configured device to the given port.
	void addDevice(const kitBase::robotModel::PortInfo &port, kitBase::robotModel::robotParts::Device *device);

private:
	kitBase::robotModel::robotParts::Device *createDevice(const kitBase::robotModel::PortInfo &port
			, const kitBase::robotModel::DeviceInfo &deviceInfo) override;

	const QString mName;
	const QString mFriendlyName;
	const int mPriority;
	QMap<kitBase::robotModel::PortInfo, kitBase::robotModel::robotParts::Device *> mPreConfiguredDevices;
};

}
}
