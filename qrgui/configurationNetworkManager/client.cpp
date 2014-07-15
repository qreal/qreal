#include "client.h"

using namespace qReal;

Client::Client() : mSettingStringSize(0)
{
	mServerSocket = new QTcpSocket(this);
	connect(mServerSocket, SIGNAL(readyRead()), this, SLOT(settings()));
	connect(mServerSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(connectionError(QAbstractSocket)));
}

Client::~Client()
{
	delete mServerSocket;
}

void Client::init()
{
	QString ip = SettingsManager::value("ServerIP").toString();
	int port = SettingsManager::value("ServerPort").toInt();

	mServerSocket->connectToHost(ip, port);
}

void Client::settings()
{
	QDataStream in(mServerSocket);
	in.setVersion(QDataStream::Qt_5_1);

	if (mSettingStringSize == 0)
	{
		if (mServerSocket->bytesAvailable() < (int)sizeof(quint16))
			return;

		in >> mSettingStringSize;
	}

	if (mServerSocket->bytesAvailable() < mSettingStringSize)
		return;

	mSettingStringSize = 0;
	QString settings;
	in >> settings;

	applySettingsFromServer(settings);
}

void Client::connectionError(QAbstractSocket::SocketError socketError)
{
	mServerSocket->close();
	emit ConfigurationNetworkManager::clientError(socketError, mServerSocket->errorString());
}

void Client::applySettingsFromServer(QString settings)
{
	int i = 0;
	while (i < settings.length())
	{
		int j = 0;
		QString key;
		while (settings[i] != '#' || settings[i + 1] != '#' || settings[i + 2] != '#')
		{
			key[j] = settings[i];
			++i;
			if (i >= settings.length() - 3)
				break;
			++j;
		}

		i = i + 3;
		if (i >= settings.length() - 3)
			break;

		j = 0;
		QString value;
		while (settings[i] != '#' || settings[i + 1] != '*' || settings[i + 2] != '#')
		{
			value[j] = settings[i];
			++i;
			++j;
			if (i >= settings.length() - 3)
				break;
		}

		if (!(value == "ServerPort" || value == "ServerIP"))
				SettingsManager::setValue(key, QVariant(value));

		i = i + 3;
		if (i >= settings.length() - 3)
			break;
	}

	mServerSocket->close();
}
