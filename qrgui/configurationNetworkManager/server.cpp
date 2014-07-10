#include "server.h"

Server::Server() :
	mTcpServer(0),
	mNetworkSession(0) {
	QNetworkConfigurationManager manager;
	if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired) {
		// Get saved network configuration
		QSettings settings(QSettings::UserScope, QLatin1String("Trolltech"));
		settings.beginGroup(QLatin1String("QtNetwork"));
		const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
		settings.endGroup();

		// If the saved network configuration is not currently discovered use the system default
		QNetworkConfiguration config = manager.configurationFromIdentifier(id);
		if ((config.state() & QNetworkConfiguration::Discovered) != QNetworkConfiguration::Discovered) {
			config = manager.defaultConfiguration();
		}

		mNetworkSession = new QNetworkSession(config, this);
		connect(mNetworkSession, SIGNAL(opened()), this, SLOT(sessionOpened()));

		mNetworkSession->open();
	} else {
		sessionOpened();
	}

	mIPAddressMapper = new QSignalMapper;
	connect(mTcpServer, SIGNAL(newConnection()), this, SLOT(acceptClientConnection()));
	connect(mIPAddressMapper, SIGNAL(mapped(QString)), this, SIGNAL(clientDisconnected(QString)));
}

void Server::sessionOpened()
{
	// Save the used configuration
	if (mNetworkSession) {
		QNetworkConfiguration config = mNetworkSession->configuration();
		QString id;
		if (config.type() == QNetworkConfiguration::UserChoice)
			id = mNetworkSession->sessionProperty(QLatin1String("UserChoiceConfiguration")).toString();
		else
			id = config.identifier();

		QSettings settings(QSettings::UserScope, QLatin1String("Trolltech"));
		settings.beginGroup(QLatin1String("QtNetwork"));
		settings.setValue(QLatin1String("DefaultNetworkConfiguration"), id);
		settings.endGroup();
	}

	mTcpServer = new QTcpServer(this);
	if (!mTcpServer->listen(QHostAddress::AnyIPv4, 55555)) {
/*		QMessageBox::critical(this, tr("qReal Server"),
							tr("Unable to start the server: %1.")
							.arg(mTcpServer->errorString()));
		close();
		return;*/
		emit serverError();
	}
}

void Server::acceptClientConnection()
{
	QTcpSocket* clientSocket = mTcpServer->nextPendingConnection();
	emit newClient(clientSocket->peerAddress().toString());
	int idusersocs = clientSocket->socketDescriptor();
	mSClients[idusersocs] = clientSocket;
	connect(mSClients[idusersocs], SIGNAL(disconnected()), mIPAddressMapper, SLOT(map()));
	mIPAddressMapper->setMapping(mSClients[idusersocs], mSClients[idusersocs]->peerAddress().toString());
	connect(mSClients[idusersocs], SIGNAL(disconnected()), mSClients[idusersocs], SLOT(deleteLater()));
	sendSettings();
}

void Server::sendSettings()
{
	if(!mSClients.isEmpty()) {
		QByteArray block;
		QDataStream out(&block, QIODevice::WriteOnly);

		out << (quint16)qReal::SettingsManager::instance()->convertToString().length();
		out << qReal::SettingsManager::instance()->convertToString();
		foreach(int i, mSClients.keys()) {
			mSClients[i]->write(block);
		}
	}
}

quint16 Server::getPort()
{
	return mTcpServer->serverPort();
}

QString Server::getIP()
{
	QList<QNetworkInterface> addressList = QNetworkInterface::allInterfaces();
	QString address;
	for (int j = 0; j < addressList.size(); j++) {
		QList<QNetworkAddressEntry> addressEntry = addressList[j].addressEntries();
		for (int i = 0; i < addressEntry.size(); i++) {
				if (!addressEntry[i].ip().isLoopback()
						&& addressEntry[i].ip().toString().contains("."))
				{
					address += addressEntry[i].ip().toString() + "\n";
				}
		}
	}

	return address;
}
