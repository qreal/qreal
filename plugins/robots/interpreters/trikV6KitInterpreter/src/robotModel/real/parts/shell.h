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

#include <trikKit/robotModel/parts/trikShell.h>
#include <utils/tcpRobotCommunicator.h>

namespace trik {
namespace robotModel {
namespace real {
namespace parts {

class Shell : public robotModel::parts::TrikShell
{
	Q_OBJECT

public:
	Shell(const kitBase::robotModel::DeviceInfo &info
			, const kitBase::robotModel::PortInfo &port
			, utils::TcpRobotCommunicator &tcpRobotCommunicator);

	void runCommand(const QString &command) override;
	void runCode(const QString &code) override;
	void say(const QString &text) override;
	void writeToFile(const QString &filePath, const QString &text) override;
	void removeFile(const QString &filePath) override;
	void print(const QString &text) override;

private:
	utils::TcpRobotCommunicator &mRobotCommunicator;
};

}
}
}
}
