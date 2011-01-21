#pragma once

#include <QtCore/QObject>
#include <QtCore/QByteArray>

class QextSerialPort;

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {

class BluetoothRobotCommunicationThread : public QObject {
	Q_OBJECT

public:
	BluetoothRobotCommunicationThread();
	~BluetoothRobotCommunicationThread();

signals:
	void connected();
	void disconnected();
	void response(QObject *addressee, QByteArray const &buffer);

public slots:
	void send(QObject *addressee, QByteArray const &buffer, unsigned const responseSize);
	void connect(QString portName);
	void reconnect(QString portName);
	void disconnect();

private:
	QextSerialPort *mPort;

};

}
}
}
}
