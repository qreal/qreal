#include "initialBlockWithPorts.h"

using namespace qReal;
using namespace interpreters::robots::details::blocks;

InitialBlockWithPorts::InitialBlockWithPorts(details::RobotModel &robotModel)
	: mRobotModel(robotModel)
{
	connect(&mRobotModel.robotImpl(), SIGNAL(connected(bool)), this, SLOT(connected(bool)));
}

void InitialBlockWithPorts::run()
{
	mRobotModel.init();
}

QList<Block::SensorPortPair> InitialBlockWithPorts::sensorToPair(QString const port1Sensor, inputPort::InputPortEnum const &port) const
{
	QString ololo = QString::fromUtf8("Ультразвуковой сенсор");
	int y = QString::compare(port1Sensor, QString::fromUtf8("Ультразвуковой сенсор"));
	bool x = (port1Sensor == QString::fromUtf8("Ультразвуковой сенсор"));
	QList<SensorPortPair> result;
	if (port1Sensor == QString::fromUtf8("Сенсор нажатия (булево значение)"))
		result << qMakePair(sensorType::touchBoolean, static_cast<int>(port));
	if (port1Sensor == QString::fromUtf8("Сенсор нажатия (сырое значение)"))
		result << qMakePair(sensorType::touchRaw, static_cast<int>(port));
	if (port1Sensor == QString::fromUtf8("Ультразвуковой сенсор"))
		result << qMakePair(sensorType::sonar, static_cast<int>(port));
	if (port1Sensor == QString::fromUtf8("Сенсор цвета (полные цвета)"))
		result << qMakePair(sensorType::colorFull, static_cast<int>(port));
	if (port1Sensor == QString::fromUtf8("Сенсор цвета (красный)"))
		result << qMakePair(sensorType::colorRed, static_cast<int>(port));
	if (port1Sensor == QString::fromUtf8("Сенсор цвета (зеленый)"))
		result << qMakePair(sensorType::colorGreen, static_cast<int>(port));
	if (port1Sensor == QString::fromUtf8("Сенсор цвета (синий)"))
		result << qMakePair(sensorType::colorBlue, static_cast<int>(port));
	if (port1Sensor == QString::fromUtf8("Сенсор цвета (пассивный)"))
		result << qMakePair(sensorType::colorNone, static_cast<int>(port));

	return result;
}

QList<Block::SensorPortPair> InitialBlockWithPorts::usedSensors() const
{
	return QList<SensorPortPair>() << sensorToPair(stringProperty("port_1"), inputPort::port1)
									<< sensorToPair(stringProperty("port_2"), inputPort::port2)
									<< sensorToPair(stringProperty("port_3"), inputPort::port3)
									<< sensorToPair(stringProperty("port_4"), inputPort::port4);
}

void InitialBlockWithPorts::connected(bool success)
{
	if (success)
		emit done(mNextBlock);
	else {
		error(tr("Can't connect to a robot."));
	}
}
