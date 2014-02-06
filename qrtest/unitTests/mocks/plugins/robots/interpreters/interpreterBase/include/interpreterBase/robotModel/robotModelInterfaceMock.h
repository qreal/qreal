#pragma once

#include <interpreterBase/robotModel/robotModelInterface.h>
#include <interpreterBase/robotModel/configuration.h>

#include <gmock/gmock.h>

namespace qrTest {

class RobotModelInterfaceMock : public interpreterBase::robotModel::RobotModelInterface
{
public:
	MOCK_CONST_METHOD0(name, QString());
	MOCK_CONST_METHOD0(friendlyName, QString());
	MOCK_METHOD0(init, void());
	MOCK_METHOD0(stopRobot, void());
	MOCK_METHOD0(disconnectFromRobot, void());
	MOCK_CONST_METHOD0(needsConnection, bool());
	MOCK_CONST_METHOD0(configuration, interpreterBase::robotModel::ConfigurationInterface const &());
	MOCK_METHOD0(mutableConfiguration, interpreterBase::robotModel::ConfigurationInterface&());
	MOCK_METHOD0(brick, interpreterBase::robotModel::robotParts::Brick&());
	MOCK_METHOD0(display, interpreterBase::robotModel::robotParts::Display&());
	MOCK_CONST_METHOD0(availablePorts, QList<interpreterBase::robotModel::PortInfo>());
	MOCK_CONST_METHOD0(configurablePorts, QList<interpreterBase::robotModel::PortInfo>());
	MOCK_CONST_METHOD1(allownDevices, QList<interpreterBase::robotModel::PluggableDeviceInfo>(
			interpreterBase::robotModel::PortInfo const &port));
};

}
