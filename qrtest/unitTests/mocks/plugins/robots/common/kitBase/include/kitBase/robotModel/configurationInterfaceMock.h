#pragma once

#include <kitBase/robotModel/configurationInterface.h>

#include <gmock/gmock.h>

namespace qrTest {

class ConfigurationInterfaceMock : public  kitBase::robotModel::ConfigurationInterface
{
public:
	MOCK_METHOD1(configureDevice, void( kitBase::robotModel::robotParts::Device * const device));

	MOCK_CONST_METHOD1(device,  kitBase::robotModel::robotParts::Device *(
			 kitBase::robotModel::PortInfo const &port));

	MOCK_CONST_METHOD0(devices, QList< kitBase::robotModel::robotParts::Device *>());

	MOCK_METHOD1(clearDevice, void( kitBase::robotModel::PortInfo const &port));
	MOCK_METHOD0(applyConfiguration, void());
};

}
