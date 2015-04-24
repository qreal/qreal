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

#include <QtCore/QObject>

#include "kitBase/kitBaseDeclSpec.h"

namespace kitBase {

/// Callback methods that enable plugin to communicate with interpreter, such as start or stop execution.
class ROBOTS_KIT_BASE_EXPORT InterpreterControlInterface : public QObject
{
	Q_OBJECT

public:
	virtual ~InterpreterControlInterface() {}

signals:
	/// Emitted when connection to robot is established, devices are configured and interpretation is started.
	void started();

	/// Emitted when program is finished or stopped by user.
	void stopped();

public slots:
	/// Starts interpteration process.
	virtual void interpret() = 0;

	/// Stops interpteration, reinitialises robot model.
	virtual void stopRobot() = 0;
};

}
