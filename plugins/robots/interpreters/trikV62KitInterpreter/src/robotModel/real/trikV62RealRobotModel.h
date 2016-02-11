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

#include <trikKit/robotModel/trikRobotModelV62.h>
#include <utils/robotCommunication/tcpRobotCommunicator.h>

namespace trik {
namespace robotModel {
namespace real {

/// @todo Refactor common code out of here.
///       It needs concerns separation: robot model is device factory and container for device-port configuration.
///       So, for different TRIK cases devices are the same, but mappings between ports and devices are
///       different. And across models devices are different (2d and real) but port mappings are the same.
///       Clearly we need to separate this into two hierarchies.
class RealRobotModel : public TrikRobotModelV62
{
	Q_OBJECT

public:
	RealRobotModel(const QString &kitId, const QString &robotId);

	QString name() const override;
	QString friendlyName() const override;
	int priority() const override;

	bool needsConnection() const override;
	void connectToRobot() override;
	void stopRobot() override;
	void disconnectFromRobot() override;

	void setErrorReporter(qReal::ErrorReporterInterface &errorReporter);

protected:
	kitBase::robotModel::robotParts::Device *createDevice(
			const kitBase::robotModel::PortInfo &port
			, const kitBase::robotModel::DeviceInfo &deviceInfo) override;

private:
	QScopedPointer<utils::robotCommunication::TcpRobotCommunicator> mRobotCommunicator;
};

}
}
}
