#include "bluetoothRobotCommunication.h"

#include "../../thirdparty/qextserialport-1.2win-alpha/qextserialenumerator.h"

#include <QtCore/QDebug>

using namespace qReal::interpreters::robots;

BluetoothRobotCommunication::BluetoothRobotCommunication()
{
}

void BluetoothRobotCommunication::send(QByteArray const &buffer)
{
	qDebug() << "Sending: ";
	qDebug() << buffer;
}

void BluetoothRobotCommunication::connect(unsigned int comPort)
{
	qDebug() << "BluetoothRobotCommunication::connect";

	QList<QextPortInfo> ports = QextSerialEnumerator::getPorts();
	qDebug() << "List of ports:";
	foreach (QextPortInfo info, ports) {
		qDebug() << "port name:" << info.portName.toLocal8Bit().constData();
		qDebug() << "friendly name:" << info.friendName.toLocal8Bit().constData();
		qDebug() << "physical name:" << info.physName.toLocal8Bit().constData();
		qDebug() << "enumerator name:" << info.enumName.toLocal8Bit().constData();
		qDebug() << "===================================";
	}
}

void BluetoothRobotCommunication::disconnect()
{
	qDebug() << "BluetoothRobotCommunication::disconnect";
}
