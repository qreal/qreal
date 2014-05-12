#pragma once

#include <interpreterBase/robotModel/configurationInterface.h>

#include <gmock/gmock.h>

namespace qrTest {

class ConfigurationInterfaceMock : public interpreterBase::robotModel::ConfigurationInterface
{
public:
	MOCK_METHOD1(configureDevice, void(interpreterBase::robotModel::robotParts::Device * const device));

	MOCK_CONST_METHOD1(device, interpreterBase::robotModel::robotParts::Device *(
			interpreterBase::robotModel::PortInfo const &port));

	MOCK_CONST_METHOD0(devices, QList<interpreterBase::robotModel::robotParts::Device *>());

	MOCK_METHOD1(clearDevice, void(interpreterBase::robotModel::PortInfo const &port));
	MOCK_METHOD0(applyConfiguration, void());
};

}
