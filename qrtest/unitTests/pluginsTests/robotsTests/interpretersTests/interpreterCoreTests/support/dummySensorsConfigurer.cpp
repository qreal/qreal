#include "dummySensorsConfigurer.h"

#include <QtCore/QDebug>

using namespace qrTest::robotsTests::interpreterCoreTests;
using namespace interpreterBase::robotModel;

DummySensorsConfigurer::DummySensorsConfigurer(QString const &name)
	: SensorsConfigurationProvider(name)
{
}

void DummySensorsConfigurer::configureSilently(QString const &robotModel
		, PortInfo const &port, DeviceInfo const &device)
{
	mCurrentConfiguration[robotModel][port] = device;
}

void DummySensorsConfigurer::configureBroadly(QString const &robotModel
		, PortInfo const &port, DeviceInfo const &device)
{
	sensorConfigurationChanged(robotModel, port, device);
}

DeviceInfo DummySensorsConfigurer::device(QString const &robotModel, PortInfo const &port) const
{
	return mCurrentConfiguration[robotModel][port];
}

void DummySensorsConfigurer::onSensorConfigurationChanged(QString const &robotModel
		, PortInfo const &port, DeviceInfo const &sensor)
{
	Q_UNUSED(robotModel)
	Q_UNUSED(port)
	Q_UNUSED(sensor)
	// This code can be uncommented to debug sensors configuration modifications
	// qDebug() << mName << "changed" << robotModel << port.toString() << "to" << sensor.toString() << this;
}
