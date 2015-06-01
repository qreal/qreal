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

#include "trikDeviceVariables.h"

#include <QtCore/QMetaClassInfo>

using namespace trik::parts;

QString TrikDeviceVariables::variableTemplatePath(const kitBase::robotModel::DeviceInfo &device
		, const kitBase::robotModel::PortInfo &port) const
{
	if (device.name() == "trikLineSensor"
			|| device.name() == "trikObjectSensor"
			|| device.name() == "trikColorSensor")
	{
		QString templateName = port.name();
		templateName.remove("Port");
		return "videosensors/" + templateName + ".t";
	} else if (device.name() == "gyroscope" || device.name() == "accelerometer") {
		return QString("%1/%2.t").arg(device.name(), port.reservedVariable());
	} else if (device.name().startsWith("gamepad")) {
		return QString("gamepad/%1.t").arg(device.name());
	}

	return generatorBase::parts::DeviceVariables::variableTemplatePath(device, port);
}
