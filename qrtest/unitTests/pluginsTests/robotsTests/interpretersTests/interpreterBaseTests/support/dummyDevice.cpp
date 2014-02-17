#include "dummyDevice.h"

using namespace qrTest::robotsTests::interpreterBaseTests;
using namespace interpreterBase::robotModel;

DummyDevice::DummyDevice(interpreterBase::robotModel::PortInfo const &port)
	: Device(DeviceInfo::create<DummyDevice>(), port)
{
}

void DummyDevice::doConfiguration()
{
	configurationCompleted(true);
}
