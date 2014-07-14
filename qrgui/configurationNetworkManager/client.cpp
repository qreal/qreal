#include "client.h"

using namespace qReal;

Client::Client() : mSettingStringSize(0)
{
	mServerSocket = new QTcpSocket(this);

	connect(mServerSocket, SIGNAL(readyRead()), this, SLOT(settings()));
	connect(mServerSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));

	QString ip = SettingsManager::value("ServerIP").toString();
	int port = SettingsManager::value("ServerPort").toInt();

	mServerSocket->connectToHost(ip, port);
}

Client::~Client()
{
	delete mServerSocket;
}

void Client::setSettings()
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

void Client::displayError(QAbstractSocket::SocketError socketError)
{
	mServerSocket->close();
	emit clientError();
//	switch (socketError)
//	{

//	case QAbstractSocket::RemoteHostClosedError:
//	{
//		break;
//	}

//	case QAbstractSocket::HostNotFoundError:
//	{
//		QMessageBox::information(this, "Client", "The host was not found. Please check the host name and port settings.");
//		break;
//	}

//	case QAbstractSocket::ConnectionRefusedError:
//	{
//		QMessageBox::information(this, "Client", "The connection was refused by the peer.  Make sure the fortune server is running,  and check that the host name and port settings are correct.");
//		break;
//	}

//	default:
//	{
//		QMessageBox::information(this, "Client", "The following error occurred: " + (mServerSocket->errorString()));
//	}

//	}
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

		SettingsManager::setValue(key, QVariant(value));

		i = i + 3;
		if (i >= settings.length() - 3)
			break;
	}
}
