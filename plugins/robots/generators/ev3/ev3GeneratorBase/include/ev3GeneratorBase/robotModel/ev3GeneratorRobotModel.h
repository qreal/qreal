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

#include <ev3Kit/robotModel/ev3RobotModelBase.h>
#include "ev3GeneratorBase/ev3GeneratorBaseDeclSpec.h"

namespace ev3 {

namespace communication {
class Ev3RobotCommunicationThread;
}

namespace robotModel {

class ROBOTS_EV3_GENERATOR_BASE_EXPORT Ev3GeneratorRobotModel : public Ev3RobotModelBase
{
	Q_OBJECT

public:
	/// Takes ownership over \a communicator.
	/// @param priority A priority of this model over other among their kit.
	Ev3GeneratorRobotModel(const QString &kitId, const QString &robotId
			, const QString &name, const QString &friendlyName, int priority
			, communication::Ev3RobotCommunicationThread * const communicator);
	~Ev3GeneratorRobotModel();

	QString name() const override;
	QString friendlyName() const override;

	bool needsConnection() const override;
	bool interpretedModel() const override;

	int priority() const override;

	/// Returns a pointer to communication thread object of this robot.
	communication::Ev3RobotCommunicationThread *communicator();

private:
	const QString mName;
	const QString mFriendlyName;
	const int mPriority;
	QScopedPointer<communication::Ev3RobotCommunicationThread> mCommunicator;
};

}
}
