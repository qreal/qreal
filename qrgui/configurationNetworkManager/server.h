#pragma once

#include "configurationNetworkManager.h"

class Server : public ConfigurationNetworkManager
{
	Q_OBJECT

public:
	explicit Server();
	~Server();

	/// returns a string, including all IPv4 addresses of this computer
	static QStringList getIP();

	/// returns port to coonect this server
	quint16 getPort();

protected:
	/// sends his qReal settings to client
	void sendSettings();

private slots:
	void sessionOpened();
	void acceptClientConnection();
	void deleteClient(int index);

signals:
	/// emits new client's IP, when connected
	void newClient(QString clientIP);

	/// emits when server is not created
	void serverError();

private:
	/// storage of all clients
	QMap<int, QTcpSocket *> mSClients;  // Doesn't take ownership
	/// TCP server
	QTcpServer *mTcpServer;  // Takes ownership
	/// network session
	QNetworkSession *mNetworkSession;  // Takes ownership
	/// this mapper addicted to each server's client and returns index of current client in QMap
	QSignalMapper *mSClientsIndexMapper;  // Takes ownership
};

