#pragma once

#include <interpreterBase/robotModel/robotModelInterface.h>
#include <interpreterBase/robotModel/configuration.h>
#include <utils/timelineInterface.h>

#include <gmock/gmock.h>

namespace qrTest {

class RobotModelInterfaceMock : public interpreterBase::robotModel::RobotModelInterface
{
	Q_OBJECT

public:
	MOCK_CONST_METHOD0(name, QString());
	MOCK_CONST_METHOD0(friendlyName, QString());

	MOCK_METHOD0(init, void());

	MOCK_METHOD0(connectToRobot, void());
	MOCK_METHOD0(stopRobot, void());
	MOCK_METHOD0(disconnectFromRobot, void());

	MOCK_CONST_METHOD0(connectionState, ConnectionState());

	MOCK_CONST_METHOD0(needsConnection, bool());

	MOCK_CONST_METHOD0(configuration
			, interpreterBase::robotModel::ConfigurationInterface const &());
	MOCK_METHOD0(mutableConfiguration, interpreterBase::robotModel::ConfigurationInterface&());

	MOCK_CONST_METHOD0(availablePorts, QList<interpreterBase::robotModel::PortInfo>());
	MOCK_CONST_METHOD0(configurablePorts, QList<interpreterBase::robotModel::PortInfo>());
	MOCK_CONST_METHOD1(allowedDevices, QList<interpreterBase::robotModel::DeviceInfo>(
			interpreterBase::robotModel::PortInfo const &port));

	MOCK_METHOD2(configureDevice, void(interpreterBase::robotModel::PortInfo const & port
			, interpreterBase::robotModel::DeviceInfo const &deviceInfo));

	MOCK_METHOD0(applyConfiguration, void());

	MOCK_CONST_METHOD0(convertibleBases, QList<interpreterBase::robotModel::DeviceInfo>());

	MOCK_METHOD0(timeline, utils::TimelineInterface &());
};

}
