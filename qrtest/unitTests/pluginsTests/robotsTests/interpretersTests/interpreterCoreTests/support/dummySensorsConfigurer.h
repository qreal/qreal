#pragma once

#include <interpreterBase/sensorsConfigurationProvider.h>

namespace qrTest {
namespace robotsTests {
namespace interpreterCoreTests {

class DummySensorsConfigurer : public interpreterBase::SensorsConfigurationProvider
{
public:
	/// Configures given device bypassing broadcasting changes to the whole graph
	void configureSilently(QString const &robotModel
			, interpreterBase::robotModel::PortInfo const &port
			, interpreterBase::robotModel::PluggableDeviceInfo const &device);

	/// Configures given device with broadcasting changes to the whole graph
	void configureBroadly(QString const &robotModel
			, interpreterBase::robotModel::PortInfo const &port
			, interpreterBase::robotModel::PluggableDeviceInfo const &device);

	interpreterBase::robotModel::PluggableDeviceInfo device(QString const &robotModel
			, interpreterBase::robotModel::PortInfo const &port);
};

}
}
}
