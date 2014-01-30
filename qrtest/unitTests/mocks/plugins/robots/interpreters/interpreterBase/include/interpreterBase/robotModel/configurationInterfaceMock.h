#pragma once

#include <interpreterBase/robotModel/configurationInterface.h>

#include <gmock/gmock.h>

namespace qrTest {

class ConfigurationInterfaceMock : public interpreterBase::robotModel::ConfigurationInterface
{
public:
	MOCK_METHOD1(configureDevice, void(interpreterBase::robotModel::robotParts::PluggableDevice * const device));
	MOCK_METHOD0(lockConfiguring, void());
	MOCK_METHOD0(unlockConfiguring, void());

	MOCK_CONST_METHOD2(pluggableDevice, interpreterBase::robotModel::robotParts::PluggableDevice *(
			interpreterBase::robotModel::PortInfo const &port
			, interpreterBase::robotModel::ConfigurationInterface::PortDirection direction)
			);

	MOCK_CONST_METHOD1(pluggableDevices, QList<interpreterBase::robotModel::robotParts::PluggableDevice *> (
			PortDirection direction)
			);

	MOCK_METHOD1(clearDevice, void(interpreterBase::robotModel::PortInfo const &port));
};

}
