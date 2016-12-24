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

#include <qrkernel/ids.h>

#include "kitBase/interpreterControlInterface.h"

namespace kitBase {

/// Interface that allows interpreter to communicate with other parts of a system.
class ROBOTS_KIT_BASE_EXPORT InterpreterInterface : public kitBase::InterpreterControlInterface
{
	Q_OBJECT

public:
	virtual ~InterpreterInterface() {}

	/// Returns time in ms that passed from the moment of last intepretation start.
	/// @warning: may return unreal time (for example when 2D model is being interpreted).
	virtual int timeElapsed() const = 0;

	/// Returns a list of diagram types that can be interpreted by this instance.
	virtual qReal::IdList supportedDiagrams() const = 0;

	virtual bool isRunning() const = 0;

signals:
	/// Emitted when connection to robot established or was broken.
	void connected(bool isConnected);

public slots:
	/// Called when establishing connection to a robot is requested. If connection is already established must
	/// disconnect from robot. May do nothing if interpretation is running, to avoid leaving robot in with motors
	/// turned on and without connection to stop it.
	virtual void connectToRobot() = 0;
};

}
