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

namespace qrTest {
namespace details {

/// Helper class that relays incoming signals. Needed to simulate different sender() for QSignalMapper when we want to
/// map different signals of the same object. See http://www.qtcentre.org/threads/39596-QSignalMapper-One-single-Object
/// for discussion of a similar task and https://github.com/minirop/SignalsSpy for similar solution.
class FakeSender : public QObject
{
	Q_OBJECT

signals:
	/// A signal that is emitted when object receives something on a "receive()" slot.
	void send();

public slots:
	/// A slot that triggers send() signal, used to relay signal with this object as a sender.
	void receive()
	{
		emit send();
	}
};

}
}
