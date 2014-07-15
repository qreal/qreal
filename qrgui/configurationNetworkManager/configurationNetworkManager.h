#pragma once

#include <QtCore/QObject>
#include <QtNetwork/QtNetwork>
#include <QtWidgets/QMessageBox>

#include <qrkernel/settingsManager.h>

class ConfigurationNetworkManager : public QObject
{
	Q_OBJECT
public:
	ConfigurationNetworkManager();
	///initialization
	virtual void init() = 0;

signals:
	///emits when error procced
	void clientError(QAbstractSocket::SocketError, QString);
};
