#include "server.h"

Server::Server():
	mTcpServer(0)
	, mNetworkSession(0)
{
	QNetworkConfigurationManager manager;
	mPort = qReal::SettingsManager::value("ServerPort").toInt();

	if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired) {
		// Get saved network configuration
		QSettings settings(QSettings::UserScope, QLatin1String("Trolltech"));
		settings.beginGroup(QLatin1String("QtNetwork"));
		QString const id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
		settings.endGroup();

		// If the saved network configuration is not currently discovered use the system default
		QNetworkConfiguration config = manager.configurationFromIdentifier(id);
		if ((config.state() & QNetworkConfiguration::Discovered) != QNetworkConfiguration::Discovered) {
			config = manager.defaultConfiguration();
		}

		mNetworkSession = new QNetworkSession(config, this);
		connect(mNetworkSession, &QNetworkSession::opened, this, &Server::sessionOpened);

		mNetworkSession->open();
	} else {
		sessionOpened();
	}

	mSClientsIndexMapper = new QSignalMapper;
	connect(mTcpServer, &QTcpServer::newConnection, this, &Server::acceptClientConnection);
	connect(mSClientsIndexMapper, static_cast<void (QSignalMapper::*)(int)>(&QSignalMapper::mapped)
			, this, &Server::deleteClient);
}

Server::~Server()
{
	delete mTcpServer;
	delete mNetworkSession;
	delete mSClientsIndexMapper;
}

void Server::sessionOpened()
{
	// Save the used configuration
	if (mNetworkSession) {
		QNetworkConfiguration config = mNetworkSession->configuration();
		QString id;
		if (config.type() == QNetworkConfiguration::UserChoice) {
			id = mNetworkSession->sessionProperty(QLatin1String("UserChoiceConfiguration")).toString();
		} else {
			id = config.identifier();
		}

		QSettings settings(QSettings::UserScope, QLatin1String("Trolltech"));
		settings.beginGroup(QLatin1String("QtNetwork"));
		settings.setValue(QLatin1String("DefaultNetworkConfiguration"), id);
		settings.endGroup();
	}

	mTcpServer = new QTcpServer(this);
	if (!mTcpServer->listen(QHostAddress::AnyIPv4, mPort)) {
		emit serverError();
	}
}

void Server::acceptClientConnection()
{
	QTcpSocket* clientSocket = mTcpServer->nextPendingConnection();
	emit newClient(clientSocket->peerAddress().toString());
	const int idUserSockets = clientSocket->socketDescriptor();
	mSClients[idUserSockets] = clientSocket;
	mSClientsIndexMapper->setMapping(mSClients[idUserSockets], idUserSockets);
	connect(mSClients[idUserSockets], &QTcpSocket::disconnected
			, mSClientsIndexMapper, static_cast<void (QSignalMapper::*)(void)>(&QSignalMapper::map));
	connect(mSClients[idUserSockets], &QTcpSocket::disconnected, mSClients[idUserSockets], &QTcpSocket::deleteLater);
	sendSettings();
}

void Server::deleteClient(int index)
{
	mSClients[index]->close();
	mSClients.remove(index);
}

void Server::sendSettings()
{
	if(!mSClients.isEmpty()) {
		QByteArray block;
		QDataStream out(&block, QIODevice::WriteOnly);

		out << (quint16)qReal::SettingsManager::instance()->convertToString().length();
		out << qReal::SettingsManager::instance()->convertToString().toUtf8();
		foreach (int i, mSClients.keys()) {
			if (mSClients[i]->isOpen()) {
				mSClients[i]->write(block);
			}
		}
	}
}

quint16 Server::getPort()
{
	return mTcpServer->serverPort();
}

QStringList Server::getIP()
{
	QList<QNetworkInterface> addressList = QNetworkInterface::allInterfaces();
	QStringList address;
	foreach (QNetworkInterface j, addressList) {
		QList<QNetworkAddressEntry> addressEntry = j.addressEntries();
		foreach (QNetworkAddressEntry i, addressEntry) {
			if (!i.ip().isLoopback()
						&& i.ip().toString().contains("."))
				{
					address.append(i.ip().toString());
				}
		}
	}

	return address;
}

void Server::changePort()
{
	mTcpServer->close();
	mPort = qReal::SettingsManager::value("ServerPort").toInt();
	if (!mTcpServer->listen(QHostAddress::AnyIPv4, mPort)) {
		emit serverError();
	}
}
