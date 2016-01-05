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

#include <QtNetwork/QTcpSocket>
#include <QtCore/QTimer>

namespace utils {
namespace robotCommunication {

class TcpConnectionHandler : public QObject
{
	Q_OBJECT

public:
	explicit TcpConnectionHandler(int port);

	bool connect(const QHostAddress &serverAddress);

	bool isConnected();

	void disconnect();

	void send(const QString &data);

signals:
	void messageReceived(const QString &message);

private slots:
	void onIncomingData();
	void onDisconnected();
	void keepalive();

private:
	QTcpSocket mSocket;
	QByteArray mBuffer;
	int mExpectedBytes = 0;
	const int mPort;

	/// Timer used to send "keepalive" packets for other side to be able to detect connection failure.
	QTimer mKeepAliveTimer;
};

}
}
