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

#include "device.h"

namespace kitBase {
namespace robotModel {
namespace robotParts {

/// Random number generator, uses C++ implementation by default and can use random device on robot if possible and
/// desired. It is needed actually to be able to control random number generation from scripts that check exercise
/// solution.
class ROBOTS_KIT_BASE_EXPORT Random : public Device
{
	Q_OBJECT
	Q_CLASSINFO("name", "random")
	Q_CLASSINFO("friendlyName", tr("Random"))
	Q_CLASSINFO("direction", "input")

	Q_PROPERTY(int fixedValue READ fixedValue WRITE setFixedValue)
	Q_PROPERTY(int callsCount READ callsCount)
	Q_PROPERTY(int lastValue READ lastValue)

public:
	/// Constructor.
	/// @param info --- device info describing this device.
	/// @param port --- port on which this device is configured.
	Random(const kitBase::robotModel::DeviceInfo &info
			, const kitBase::robotModel::PortInfo &port);

	/// Provides integer random number in a given interval. It also counts calls, to be able to check that user
	/// actually used RNG.
	virtual int random(int from, int to);

	/// Sets given value as a value always returned by "random", as in
	/// "return 4;  // Absolutely random, checked with dice"
	/// It will be returned even when it is out of [from; to] range, to avoid cheating with "from" and "to" parameters
	/// when solving exercise.
	void setFixedValue(int value);

	/// Getter for fixed value of random number generator if it is set.
	int fixedValue() const;

	/// Returns calls count of "random" method.
	int callsCount() const;

	/// Returns last value returned by "random" method if it has been called, 0 otherwise.
	int lastValue() const;

private:
	/// Calls count of "random" method.
	int mCallsCount = 0;

	/// Fixed value which shall be returned by "random" method if generator is in fixed mode.
	int mFixedValue = 0;

	/// Shall or shall not this generator return fixed value every time "random" is called.
	bool mIsFixed = false;

	int mLastValue = 0;
};

}
}
}
