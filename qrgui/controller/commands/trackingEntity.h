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

#include "qrgui/controller/controllerDeclSpec.h"

namespace qReal {
namespace commands {

/// A helper class for commands that track some continious user operation
class QRGUI_CONTROLLER_EXPORT TrackingEntity
{
public:
	virtual ~TrackingEntity() {}

	/// Executed when user starts some operation
	virtual void startTracking();

	/// Executed when user has finished some operation
	virtual void stopTracking();

	/// Executed when for some reason this command mustn`t handle user`s operation
	virtual void rejectTracking();

protected:
	bool mTrackStarted;
	bool mTrackStopped;
};

}
}
