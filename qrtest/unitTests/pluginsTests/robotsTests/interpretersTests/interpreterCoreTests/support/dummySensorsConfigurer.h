#pragma once

#include <interpreterBase/devicesConfigurationProvider.h>

namespace qrTest {
namespace robotsTests {
namespace interpreterCoreTests {

class DummySensorsConfigurer : public interpreterBase::DevicesConfigurationProvider
{
public:
	explicit DummySensorsConfigurer(QString const &name = QString());

	/// Configures given device with broadcasting changes to the whole graph
	void configureBroadly(QString const &robotModel
			, interpreterBase::robotModel::PortInfo const &port
			, interpreterBase::robotModel::DeviceInfo const &device);

	interpreterBase::robotModel::DeviceInfo device(QString const &robotModel
			, interpreterBase::robotModel::PortInfo const &port) const;

protected:
	void onDeviceConfigurationChanged(QString const &robotModel
			, interpreterBase::robotModel::PortInfo const &port
			, interpreterBase::robotModel::DeviceInfo const &sensor) override;
};

}
}
}
