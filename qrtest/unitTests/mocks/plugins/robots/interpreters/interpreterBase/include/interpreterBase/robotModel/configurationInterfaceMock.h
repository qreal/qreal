#pragma once

#include <interpreterBase/robotModel/configurationInterface.h>

#include <gmock/gmock.h>

namespace qrTest {

class ConfigurationInterfaceMock : public interpreterBase::robotModel::ConfigurationInterface
{
public:
	MOCK_METHOD1(configureDevice, void(interpreterBase::robotModel::robotParts::Device * const device));

	MOCK_CONST_METHOD2(device, interpreterBase::robotModel::robotParts::Device *(
			interpreterBase::robotModel::PortInfo const &port
			, interpreterBase::robotModel::ConfigurationInterface::PortDirection direction)
			);

	MOCK_CONST_METHOD1(devices, QList<interpreterBase::robotModel::robotParts::Device *> (
			PortDirection direction)
			);

	MOCK_METHOD1(clearDevice, void(interpreterBase::robotModel::PortInfo const &port));
	MOCK_METHOD0(applyConfiguration, void());
};

}
