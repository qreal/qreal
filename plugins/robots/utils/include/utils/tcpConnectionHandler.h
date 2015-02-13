#pragma once

#include <QtNetwork/QTcpSocket>

#include "utilsDeclSpec.h"

namespace utils {

class ROBOTS_UTILS_EXPORT TcpConnectionHandler : public QObject
{
	Q_OBJECT

public:
	explicit TcpConnectionHandler(int port);

	bool connect(const QHostAddress &serverAddress);

	bool isConnected();

	void disconnect();

	void send(const QString &data);

public slots:
	void onIncomingData();

signals:
	void messageReceived(const QString &message);

private:
	QTcpSocket mSocket;
	QByteArray mBuffer;
	int mExpectedBytes = 0;
	int mPort;
};

}
