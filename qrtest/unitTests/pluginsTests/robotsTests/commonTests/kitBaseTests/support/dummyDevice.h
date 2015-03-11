#pragma once

#include <kitBase/robotModel/robotParts/device.h>

namespace qrTest {
namespace robotsTests {
namespace kitBaseTests {

/// Minimal implementation of device sufficient for testing.
class DummyDevice : public kitBase::robotModel::robotParts::Device
{
	Q_OBJECT

public:
	explicit DummyDevice(kitBase::robotModel::PortInfo const &port);

protected:
	void doConfiguration() override;
};

}
}
}
