#pragma once

#include <QtNetwork/QTcpSocket>

#include "utilsDeclSpec.h"

namespace utils {

class ROBOTS_UTILS_EXPORT TcpConnectionHandler : public QObject
{
	Q_OBJECT
public:
	explicit TcpConnectionHandler(int port);

	bool connect(QHostAddress const &serverAddress);

	bool isConnected();

	void disconnect();

	void send(QString const &data);

public slots:
	void onIncomingData();

signals:
	void messageReceived(QString const &message);

private:
	QTcpSocket mSocket;
	QByteArray mBuffer;
	int mExpectedBytes = 0;
	int mPort;
};

}
