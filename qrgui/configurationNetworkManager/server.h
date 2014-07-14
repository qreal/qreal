#pragma once

#include "configurationNetworkManager.h"

#include <QtNetwork/QtNetwork>
#include <QtWidgets/QMessageBox>
#include <QtNetwork/QNetworkSession>

#include "qrkernel/settingsManager.h"

class Server : public ConfigurationNetworkManager
{
	Q_OBJECT

public:
	explicit Server();

	/// returns a string, including all IPv4 addresses of this computer
	static QString getIP();

	/// returns port to coonect this server
	quint16 getPort();

protected:
	/// sends his qReal settings to client
	void sendSettings();

private slots:
	void sessionOpened();
	void acceptClientConnection();

signals:
	/// emits new client's IP, when connected
	void newClient(QString clientIP);

	/// emits IP of disconnected client
	void clientDisconnected(QString clientsIP);

	/// emits when server is not created
	void serverError();

private:
	/// storage of all clients
	QMap<int,QTcpSocket *> mSClients;
	QTcpServer *mTcpServer;
	QNetworkSession *mNetworkSession;
	QSignalMapper *mIPAddressMapper;
};

