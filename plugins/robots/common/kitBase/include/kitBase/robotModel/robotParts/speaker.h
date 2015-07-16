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

#include "device.h"
#include "kitBase/kitBaseDeclSpec.h"

namespace kitBase {
namespace robotModel {
namespace robotParts {

/// Base class for speaker or line-out device.
class ROBOTS_KIT_BASE_EXPORT Speaker : public Device
{
	Q_OBJECT
	Q_CLASSINFO("name", "speaker")
	Q_CLASSINFO("friendlyName", tr("Speaker"))
	Q_CLASSINFO("direction", "output")

public:
	/// Constructor, takes device type info and port on which this device is configured.
	Speaker(const DeviceInfo &info, const PortInfo &port);

signals:
	/// Emitted when some error occured during sound playing process.
	void error(const QString &message);
};

}
}
}
