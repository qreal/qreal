#pragma once

#include "interpreterBase/robotModel/robotParts/device.h"

namespace qrTest {
namespace robotsTests {
namespace interpreterBaseTests {

/// Minimal implementation of device sufficient for testing.
class DummyDevice : public interpreterBase::robotModel::robotParts::Device
{
	Q_OBJECT

public:
	explicit DummyDevice(interpreterBase::robotModel::PortInfo const &port);

protected:
	void doConfiguration() override;
};

}
}
}
