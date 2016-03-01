/* Copyright 2016 CyberTech Labs Ltd.
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
#include <QtNetwork/QAbstractSocket>

class QHostAddress;
class QTcpSocket;
class QTimer;

namespace tcpRobotSimulator {

/// Connection protocol variants.
enum class Protocol
{
	/// Message is in form "<data length in bytes>:<data>".
	messageLength

	/// Message is in form "<data>\n".
	, endOfLineSeparator
};

/// Heartbeat protocol option.
enum class Heartbeat
{
	/// Wait for a packet every N milliseconds, if none is received, assume connection lost and close socket.
	use

	/// Do not use heartbeat, keep socket open until TCP protocol detects disconnect (which may take a while,
	/// so we will not be able to detect hardware network failures).
	, dontUse
};

class Connection: public QObject
{
	Q_OBJECT

public:
	/// Constructor.
	/// @param connectionProtocol - protocol used by this connection.
	/// @param useHeartbeat - use or don't use heartbeat protocol option.
	/// @param configVersion - version of a config file that this connection needs to simulate. Will be used to answer
	///        "configVersion" request.
	Connection(Protocol connectionProtocol, Heartbeat useHeartbeat, const QString &configVersion);

	~Connection() override;

	/// Creates socket and initializes incoming connection, shall be called when Connection is already in its own
	/// thread.
	/// @param socketDescriptor - native socket descriptor.
	Q_INVOKABLE void init(int socketDescriptor);

	/// Sends given byte array to peer.
	Q_INVOKABLE void send(const QByteArray &data);

	Q_INVOKABLE void closeConnection();

	bool runProgramRequestReceived() const;

	bool configVersionRequestReceived() const;

	bool versionRequestReceived() const;

signals:
	/// Emitted after connection becomes closed.
	void disconnected();

	/// Emitted when connection receives command to run the program.
	void runProgramRequestReceivedSignal();

protected:
	/// Creates socket and initializes outgoing connection, shall be called when Connection is already in its own
	/// thread.
	/// @param ip - target ip address.
	/// @param port - target port.
	void init(const QHostAddress &ip, int port);

private slots:
	/// New data is ready on a socket.
	void onReadyRead();

	/// Socket is opened.
	void onConnect();

	/// Socket is closed for some reason.
	void onDisconnect();

	/// Socket is failed to connect or some other error occured.
	void onError(QAbstractSocket::SocketError error);

	/// Sends "keepalive" packet.
	void keepAlive();

	/// Heartbeat timer timed out, close connection.
	void onHeartbeatTimeout();

private:
	/// Processes received data.
	virtual void processData(const QByteArray &data);

	/// Handles incoming data: sending version or processing received data.
	void handleIncomingData(const QByteArray &data);

	/// Connects all slots of this object to the appropriate signals.
	void connectSlots();

	/// Parses current buffer content and splits it on complete messages.
	void processBuffer();

	/// Helper method that notifies everyone about disconnect and closes connection gracefully.
	void doDisconnect();

	/// Initializes keepalive and heartbeat timers.
	void initKeepalive();

	/// Socket for this connection.
	QScopedPointer<QTcpSocket> mSocket;

	/// Buffer to accumulate parts of a message.
	QByteArray mBuffer;

	/// Declared size of a current message.
	int mExpectedBytes = 0;

	/// Protocol selected for this connection.
	Protocol mProtocol;

	/// Timer that is used to send keepalive packets.
	QScopedPointer<QTimer> mKeepAliveTimer;

	/// Timer that is used to check that keepalive packets from other end of the line were properly received.
	QScopedPointer<QTimer> mHeartbeatTimer;

	/// Flag that ensures that "disconnected" signal will be sent only once.
	bool mDisconnectReported = false;

	/// Use or don;t use heartbeat method of connection loss detection.
	bool mUseHeartbeat = false;

	/// Boolean flag that becomes true when we receive "run" command.
	bool mRunProgramRequestReceived = false;

	/// Boolean flag that becomes true when we receive "configVersion" command.
	bool mConfigVersionRequestReceived = false;

	/// Boolean flag that becomes true when we receive "version" command.
	bool mVersionRequestReceived = false;

	/// Simulated config version.
	const QString mConfigVersion;
};

}
