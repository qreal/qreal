#pragma once

#include <QtCore/QString>
#include <QtCore/QThread>

#include "utils/utilsDeclSpec.h"
#include "robotCommunicationThreadInterface.h"

class QextSerialPort;

namespace utils {
namespace robotCommunication {

class ROBOTS_UTILS_EXPORT RobotCommunicator : public QObject
{
	Q_OBJECT

public:
	explicit RobotCommunicator(QObject *parent = 0);
	~RobotCommunicator();

	void send(QObject *addressee, const QByteArray &buffer, const unsigned responseSize);
	void send(const QByteArray &buffer, const unsigned responseSize, QByteArray &outputBuffer);
	void connect();
	void disconnect();

	void setRobotCommunicationThreadObject(RobotCommunicationThreadInterface *robotCommunication);

	/// Checks if connection can be established or emits errorOccured();
	void checkConsistency();

signals:
	void errorOccured(const QString &message);
	void connected(bool success, const QString &errorString);
	void disconnected();
	void response(QObject *addressee, const QByteArray &buffer);

private:
	QThread mRobotCommunicationThread;
	RobotCommunicationThreadInterface *mRobotCommunicationThreadObject;
};

}
}
