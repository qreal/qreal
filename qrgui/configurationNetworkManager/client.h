#pragma once

#include "configurationNetworkManager.h"
#include "mainwindow/mainWindow.h"

class Client : public ConfigurationNetworkManager
{
	Q_OBJECT

public:
	Client();
	~Client();

	//	void init() override;

private slots:
	///get and apply settings from server
	void settings();
	///close session when connection failed
	void connectionError(QAbstractSocket::SocketError socketError);

private:
	///apply setting from server
	void applySettingsFromServer(QString settings);
	///server socket
	QTcpSocket *mServerSocket;
	///size of string which send from server
	quint16 mSettingStringSize;
};
