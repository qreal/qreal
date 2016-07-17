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

#include "device.h"

namespace kitBase {
namespace robotModel {
namespace robotParts {

/// Base class for command shells.
class ROBOTS_KIT_BASE_EXPORT Shell : public Device
{
	Q_OBJECT
	Q_CLASSINFO("name", "shell")
	Q_CLASSINFO("friendlyName", tr("Shell"))
	Q_CLASSINFO("direction", "output")

public:
	Shell(const kitBase::robotModel::DeviceInfo &info
			, const kitBase::robotModel::PortInfo &port);

	/// Prints the given text into virtual or real robot console.
	virtual void print(const QString &text) = 0;

signals:
	/// Emitted when some text is added into robot`s console output.
	void textPrinted(const QString &text);

	/// Emitted when robot have read file and have sent its contents.
	void fileContents(const QString &fileContents);

	/// Emitted when new mail arrives from robot.
	void mailArrived(const QString &mail);
};

}
}
}
