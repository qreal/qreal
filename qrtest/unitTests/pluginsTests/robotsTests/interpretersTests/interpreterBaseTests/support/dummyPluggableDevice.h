#pragma once

#include "interpreterBase/robotModel/robotParts/pluggableDevice.h"

namespace qrTest {
namespace robotsTests {
namespace interpreterBaseTests {

/// Minimal implementation of pluggable device sufficient for testing.
class DummyPluggableDevice : public interpreterBase::robotModel::robotParts::PluggableDevice
{
	Q_OBJECT

public:
	DummyPluggableDevice(interpreterBase::robotModel::PortInfo const &port);

	interpreterBase::robotModel::PluggableDeviceInfo const &deviceInfo() const override;

protected:
	void doConfiguration() override;
};

}
}
}
