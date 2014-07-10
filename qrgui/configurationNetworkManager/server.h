#pragms once

#include "configurationNetworkManager.h"

#include <QtNetwork>
#include <QMessageBox>

#include "qrkernel/settingsManager.h"

class Server : ConfigurationNetworkManager
{
	Q_OBJECT

public:
	explicit Server();
	/// returns a string, including all IPv4 addresses of this computer
	QString getIP();
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

private:
	/// storage of all clients
	QMap<int,QTcpSocket *> mSClients;
	QTcpServer *mTcpServer;
	QNetworkSession *mNetworkSession;
	QSignalMapper *mIPAddressMapper;
};

