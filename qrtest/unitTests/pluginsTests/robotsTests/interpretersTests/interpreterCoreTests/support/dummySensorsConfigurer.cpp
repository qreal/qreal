#include "dummySensorsConfigurer.h"

#include <QtCore/QDebug>

using namespace qrTest::robotsTests::interpreterCoreTests;
using namespace interpreterBase::robotModel;

DummySensorsConfigurer::DummySensorsConfigurer(QString const &name)
	: DevicesConfigurationProvider(name)
{
}

void DummySensorsConfigurer::configureBroadly(QString const &robotModel
		, PortInfo const &port, DeviceInfo const &device)
{
	deviceConfigurationChanged(robotModel, port, device);
}

DeviceInfo DummySensorsConfigurer::device(QString const &robotModel, PortInfo const &port) const
{
	return currentConfiguration(robotModel, port);
}

void DummySensorsConfigurer::onDeviceConfigurationChanged(QString const &robotModel
		, PortInfo const &port, DeviceInfo const &sensor)
{
	Q_UNUSED(robotModel)
	Q_UNUSED(port)
	Q_UNUSED(sensor)
	// This code can be uncommented to debug sensors configuration modifications
	// qDebug() << mName << "changed" << robotModel << port.toString() << "to" << sensor.toString() << this;
}
