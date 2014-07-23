#pragma once

#include "configurationNetworkManager.h"
#include "mainwindow/mainWindow.h"

class Client : public ConfigurationNetworkManager
{
	Q_OBJECT

public:
	Client();
	~Client();

private slots:
	///get and apply settings from server
	void settings();
	///close session when connection failed
	void connectionError();

signals:
	void mustDeleteClient();

public:
	void connectToSettingsServer();

private:
	///apply setting from server
	void applySettingsFromServer(QString const settings);
	///server socket
	QTcpSocket *mServerSocket;  // Takes ownership
	///size of string which send from server
	quint16 mSettingStringSize;
};
