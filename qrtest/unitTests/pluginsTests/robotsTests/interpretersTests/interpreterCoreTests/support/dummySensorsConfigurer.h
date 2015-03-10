#pragma once

#include <kitBase/devicesConfigurationProvider.h>

namespace qrTest {
namespace robotsTests {
namespace interpreterCoreTests {

class DummySensorsConfigurer : public kitBase::DevicesConfigurationProvider
{
public:
	explicit DummySensorsConfigurer(QString const &name = QString());

	/// Configures given device with broadcasting changes to the whole graph
	void configureBroadly(QString const &robotModel
			, kitBase::robotModel::PortInfo const &port
			, kitBase::robotModel::DeviceInfo const &device);

	kitBase::robotModel::DeviceInfo device(QString const &robotModel
			, kitBase::robotModel::PortInfo const &port) const;

protected:
	void onDeviceConfigurationChanged(QString const &robotModel
			, kitBase::robotModel::PortInfo const &port
			, kitBase::robotModel::DeviceInfo const &sensor
			, kitBase::DevicesConfigurationProvider::Reason reason) override;
};

}
}
}
