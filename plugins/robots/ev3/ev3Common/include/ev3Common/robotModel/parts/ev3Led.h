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

#include <kitBase/robotModel/robotParts/device.h>

namespace ev3 {
namespace robotModel {
namespace parts {

/// A enumeration of all modes available for EV3 LED.
enum class Ev3LedColor
{
	/// LED is turned off, black color
	off = 0
	/// LED is turned on, red color
	, red
	/// LED is turned on, green color
	, green
	/// LED is turned on, orange color
	, orange
	/// LED is flashes with red color
	, redFlash
	/// LED is flashes with green color
	, greenFlash
	/// LED is flashes with orange color
	, orangeFlash
	/// LED is pulses with red color
	, redPulse
	/// LED is pulses with green color
	, greenPulse
	/// LED is pulses with orange color
	, orangePulse
};

/// A base for all implementations of LED on the front panel of EV3 brick.
class Ev3Led : public kitBase::robotModel::robotParts::Device
{
	Q_OBJECT
	Q_CLASSINFO("name", "led")
	Q_CLASSINFO("friendlyName", tr("LED"))
	Q_CLASSINFO("direction", "output")

public:
	Ev3Led(const kitBase::robotModel::DeviceInfo &info
			, const kitBase::robotModel::PortInfo &port);

	virtual void setColor(Ev3LedColor color) = 0;
};

}
}
}
