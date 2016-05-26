/* Copyright 2016 Grigorii Zimin
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

/// Abstract motorsAggregator which can be turned on with given speed or turned off.
class ROBOTS_KIT_BASE_EXPORT MotorsAggregator : public Device
{
	Q_OBJECT
	Q_CLASSINFO("name", "motorsAggregator")
	Q_CLASSINFO("friendlyName", tr("Motors aggregator"))
	Q_CLASSINFO("direction", "output")

public:
	/// Constructor, takes device type info and port on which motorsAggragator is configured.
	MotorsAggregator(const DeviceInfo &info, const PortInfo &port);

	/// Returns last speed setted to a motor with given port via 'on', 'stop' or 'off' methods.
	int power(const QString &port) const;

	/// Turn on all given motors with given speeds (commonly in range -100..100, but it may depend on concrete motor).
	virtual void on(const QList<QPair<QString, int>> &powerForMotors);

	/// Stops given motor actively, with breaking. For NXT and TRIK it is equivalent to on({QPair{port,0},...})
	/// , but may depend on motor construction.
	/// Note that motor still be on and in most real motors it even will consume maximal amount of power
	/// in this mode (somewhat counter-intuitive).
	virtual void stop(const QStringList &ports);

	/// Turns off electrical supply, which stops given motor without breaking. Note that on real motors it is completely
	/// different from "stop()", but in 2D model "stop()" and "off()" behave basically the same way (some differences
	/// may be noted in advanced physics mode, related to breaking).
	virtual void off(const QStringList &ports);

private:
	QMap<QString, int> mSpeeds;
};

}
}
}
