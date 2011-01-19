#include "bluetoothRobotCommunication.h"

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
}

void BluetoothRobotCommunication::disconnect()
{
	qDebug() << "BluetoothRobotCommunication::disconnect";
}
