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
#include <QtCore/QByteArray>

#include "utils/utilsDeclSpec.h"

namespace utils {
namespace robotCommunication {

/// An interface for each concrete robots communication protocol implementation
class ROBOTS_UTILS_EXPORT RobotCommunicationThreadInterface : public QObject
{
	Q_OBJECT

public:
	virtual ~RobotCommunicationThreadInterface() {}

public slots:
	virtual void send(QObject *addressee, const QByteArray &buffer, const unsigned responseSize) = 0;
	virtual void send(const QByteArray &buffer, const unsigned responseSize, QByteArray &outputBuffer) = 0;
	virtual void connect() = 0;
	virtual void disconnect() = 0;
	virtual void reconnect() = 0;
	virtual void allowLongJobs(bool allow = true) = 0;
	virtual void checkConsistency() = 0;

signals:
	void connected(bool success, const QString &errorString);
	void disconnected();
	void response(QObject *addressee, const QByteArray &buffer);
	void errorOccured(const QString &message);
};

}
}
