#pragma once

#include "configurationNetworkManager.h"

class Client : public ConfigurationNetworkManager
{
	Q_OBJECT

public:
	Client();
	~Client();

private slots:
	///get and apply settings from server
	void settings();
	void error(QAbstractSocket::SocketError socketError);

signals:
	void clientError(QAbstractSocket::SocketError socketError, QString error);

private:
	///apply setting from server
	void applySettingsFromServer(QString settings);
	///server socket
	QTcpSocket *mServerSocket;
	///size of string which send from server
	quint16 mSettingStringSize;
};
