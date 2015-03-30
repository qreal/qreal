#include "dummyDevice.h"

using namespace qrTest::robotsTests::kitBaseTests;
using namespace kitBase::robotModel;

DummyDevice::DummyDevice(kitBase::robotModel::PortInfo const &port)
	: Device(DeviceInfo::create<DummyDevice>(), port)
{
}

void DummyDevice::doConfiguration()
{
	configurationCompleted(true);
}
