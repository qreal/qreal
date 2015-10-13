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

#include <kitBase/robotModel/robotParts/shell.h>

namespace trik {
namespace robotModel {
namespace parts {

/// Executes shell commands on TRIK controller
class TrikShell : public kitBase::robotModel::robotParts::Shell
{
	Q_OBJECT

public:
	/// Constructor, takes device type info and port on which this motor is configured.
	TrikShell(const kitBase::robotModel::DeviceInfo &info
			, const kitBase::robotModel::PortInfo &port);
	virtual ~TrikShell();

	/// Run current command in the trik
	virtual void runCommand(const QString &command) = 0;

	/// Evaluates the given code on native runtime language.
	virtual void runCode(const QString &code) = 0;

	/// Force trik say the text
	virtual void say(const QString &text) = 0;

	/// Writes the text to a given file
	virtual void writeToFile(const QString &filePath, const QString &text) = 0;

	/// Removes a given file
	virtual void removeFile(const QString &filePath) = 0;
};

}
}
}
