#include "dummySensorsConfigurer.h"

#include <QtCore/QDebug>

using namespace qrTest::robotsTests::interpreterCoreTests;
using namespace kitBase::robotModel;

DummySensorsConfigurer::DummySensorsConfigurer(QString const &name)
	: DevicesConfigurationProvider(name)
{
}

void DummySensorsConfigurer::configureBroadly(QString const &robotModel
		, PortInfo const &port, DeviceInfo const &device)
{
	deviceConfigurationChanged(robotModel, port, device, Reason::userAction);
}

DeviceInfo DummySensorsConfigurer::device(QString const &robotModel, PortInfo const &port) const
{
	return currentConfiguration(robotModel, port);
}

void DummySensorsConfigurer::onDeviceConfigurationChanged(QString const &robotModel
		, PortInfo const &port, DeviceInfo const &sensor, Reason reason)
{
	Q_UNUSED(robotModel)
	Q_UNUSED(port)
	Q_UNUSED(sensor)
	Q_UNUSED(reason)
	// This code can be uncommented to debug sensors configuration modifications
	// qDebug() << mName << "changed" << robotModel << port.toString() << "to" << sensor.toString() << this;
}
