#include "dummySensorsConfigurer.h"

using namespace qrTest::robotsTests::interpreterCoreTests;
using namespace interpreterBase::robotModel;

void DummySensorsConfigurer::configureSilently(QString const &robotModel
		, PortInfo const &port, PluggableDeviceInfo const &device)
{
	mCurrentConfiguration[robotModel][port] = device;
}

void DummySensorsConfigurer::configureBroadly(QString const &robotModel
		, PortInfo const &port, PluggableDeviceInfo const &device)
{
	configureSilently(robotModel, port, device);
	onSensorConfigurationChanged(robotModel, port, device);
}

PluggableDeviceInfo DummySensorsConfigurer::device(QString const &robotModel, PortInfo const &port)
{
	return mCurrentConfiguration[robotModel][port];
}
