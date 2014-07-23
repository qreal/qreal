#include "client.h"

using namespace qReal;

Client::Client() : mSettingStringSize(0)
{
	mServerSocket = new QTcpSocket(this);
	connect(mServerSocket, &QAbstractSocket::readyRead, this, &Client::settings);
	connect(mServerSocket, static_cast<void (QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error), this, &Client::connectionError);
	connectToSettingsServer();
}

Client::~Client()
{
	if (mServerSocket->isOpen()) {
		mServerSocket->close();
	}
	delete mServerSocket;
}

void Client::connectToSettingsServer()
{
	const QString ip = SettingsManager::value("ServerIP").toString();
	const int port = SettingsManager::value("ServerPort").toInt();

	mServerSocket->connectToHost(ip, port);
}

void Client::settings()
{
	QDataStream in(mServerSocket);
	in.setVersion(QDataStream::Qt_5_1);

	if (mSettingStringSize == 0) {
		if (mServerSocket->bytesAvailable() < (int)sizeof(quint16)) {
			return;
		}

		in >> mSettingStringSize;
	}

	if (mServerSocket->bytesAvailable() < mSettingStringSize) {
		return;
	}

	mSettingStringSize = 0;
	
	char *settings = nullptr;
	in >> settings;
	mServerSocket->close();
	
	QString str = QString::fromUtf8(settings);
	applySettingsFromServer(str);
	delete settings;
	emit mustDeleteClient();
}

void Client::connectionError()
{
	mServerSocket->close();
}

void Client::applySettingsFromServer(const QString settings)
{
	int i = 0;
	while (i < settings.length()) {
		int j = 0;
		QString key;
		while (settings[i] != '#' || settings[i + 1] != '#' || settings[i + 2] != '#') {
			key[j] = settings[i];
			++i;
			if (i >= settings.length() - 3)
				break;
			++j;
		}

		i = i + 3;
		if (i >= settings.length() - 3) {
			break;
		}

		j = 0;
		QString value;
		while (settings[i] != '#' || settings[i + 1] != '*' || settings[i + 2] != '#') {
			value[j] = settings[i];
			++i;
			++j;
			if (i >= settings.length() - 3)
				break;
		}

		if (!(key == "ServerPort" || key == "ServerIP")) {
			SettingsManager::setValue(key, QVariant(value));
		}

		i = i + 3;
		if (i >= settings.length() - 3) {
			break;
		}
	}
}
