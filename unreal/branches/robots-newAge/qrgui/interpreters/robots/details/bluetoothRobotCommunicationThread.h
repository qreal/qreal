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
	void response(QByteArray const &buffer);

public slots:
	void send(QByteArray const &buffer);
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
