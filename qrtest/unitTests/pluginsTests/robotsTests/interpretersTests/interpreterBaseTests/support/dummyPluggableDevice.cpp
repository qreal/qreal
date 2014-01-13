#include "dummyPluggableDevice.h"

using namespace qrTest::robotsTests::interpreterBaseTests;
using namespace interpreterBase::robotModel;

DummyPluggableDevice::DummyPluggableDevice(interpreterBase::robotModel::PortInfo const &port)
	: PluggableDevice(port)
{
}

void DummyPluggableDevice::doConfiguration()
{
	configurationCompleted(true);
}
