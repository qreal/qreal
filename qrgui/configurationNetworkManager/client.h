#pragma once

#include "configurationNetworkManager.h"

class Client : public ConfigurationNetworkManager
{
	Q_OBJECT

public:
	Client();

private slots:
	///get and apply settings from server
	void setSettings();
	///show error if connecttion failed
	void displayError(QAbstractSocket::SocketError socketError);

signals:
	void clientError();

private:
	///apply setting from server
	void applySettingsFromServer(QString settings);
	///server socket
	QTcpSocket *mServerSocket;
	///size of string which send from server
	quint16 mSettingStringSize;
};
