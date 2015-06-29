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

#include <kitBase/robotModel/robotParts/speaker.h>
#include <utils/robotCommunication/robotCommunicator.h>

namespace trik {
namespace robotModel {
namespace parts {

class TrikSpeaker : public kitBase::robotModel::robotParts::Speaker
{
	Q_OBJECT

public:
	TrikSpeaker(const kitBase::robotModel::DeviceInfo &info
			, const kitBase::robotModel::PortInfo &port);

	/// Plays the given sound file asynchroniously. When sound file is played to the end finished() signal
	/// will be emitted. If something went wrong error() signal will be thrown with message as argument.
	/// @todo: Implement both asynchronious and synchronious modes?
	virtual void play(const QString &filePath) = 0;

signals:
	/// Emitted when currently played sound file has been played to the end.
	void finished();
};

}
}
}
