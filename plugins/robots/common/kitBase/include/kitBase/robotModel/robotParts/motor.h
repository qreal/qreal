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

/// Abstract motor which can be turned on with given speed or turned off.
class ROBOTS_KIT_BASE_EXPORT Motor : public Device
{
	Q_OBJECT
	Q_CLASSINFO("name", "motor")
	Q_CLASSINFO("friendlyName", tr("Motor"))
	Q_CLASSINFO("direction", "output")

	Q_PROPERTY(int power READ power WRITE on)

public:
	/// Constructor, takes device type info and port on which this motor is configured.
	Motor(const DeviceInfo &info, const PortInfo &port);

	/// Returns last speed setted to a motor via 'on', 'stop' or 'off' methods.
	int power() const;

	/// Turn on a motor with given speed (commonly in range -100..100, but it may depend on concrete motor).
	virtual void on(int speed);

	/// Stops motor actively, with breaking. For NXT and TRIK it is equivalent to on(0), but may depend on motor
	/// construction. Note that motor still be on and in most real motors it even will consume maximal amount of power
	/// in this mode (somewhat counter-intuitive).
	virtual void stop();

	/// Turns off electrical supply, which stops motor without breaking. Note that on real motors it is completely
	/// different from "stop()", but in 2D model "stop()" and "off()" behave basically the same way (some differences
	/// may be noted in advanced physics mode, related to breaking).
	virtual void off();

private:
	int mSpeed;
};

}
}
}
