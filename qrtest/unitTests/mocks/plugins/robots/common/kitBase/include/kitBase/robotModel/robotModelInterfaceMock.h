#pragma once

#include <kitBase/robotModel/robotModelInterface.h>
#include <kitBase/robotModel/configuration.h>
#include <utils/timelineInterface.h>

#include <gmock/gmock.h>

namespace qrTest {

class RobotModelInterfaceMock : public kitBase::robotModel::RobotModelInterface
{
	Q_OBJECT

public:
	MOCK_CONST_METHOD0(robotId, QString());
	MOCK_CONST_METHOD0(name, QString());
	MOCK_CONST_METHOD0(friendlyName, QString());
	MOCK_CONST_METHOD0(kitId, QString());
	MOCK_CONST_METHOD0(priority, int());
	MOCK_CONST_METHOD0(interpretedModel, bool());

	MOCK_METHOD0(init, void());

	MOCK_METHOD0(connectToRobot, void());
	MOCK_METHOD0(stopRobot, void());
	MOCK_METHOD0(disconnectFromRobot, void());

	MOCK_CONST_METHOD0(connectionState, ConnectionState());

	MOCK_CONST_METHOD0(needsConnection, bool());

	MOCK_CONST_METHOD0(configuration
			, kitBase::robotModel::ConfigurationInterface const &());
	MOCK_METHOD0(mutableConfiguration,  kitBase::robotModel::ConfigurationInterface&());

	MOCK_CONST_METHOD0(availablePorts, QList< kitBase::robotModel::PortInfo>());
	MOCK_CONST_METHOD0(configurablePorts, QList< kitBase::robotModel::PortInfo>());
	MOCK_CONST_METHOD1(allowedDevices, QList< kitBase::robotModel::DeviceInfo>(
			kitBase::robotModel::PortInfo const &port));

	MOCK_METHOD2(configureDevice, void( kitBase::robotModel::PortInfo const & port
			, kitBase::robotModel::DeviceInfo const &deviceInfo));

	MOCK_METHOD0(applyConfiguration, void());

	MOCK_CONST_METHOD0(convertibleBases, QList< kitBase::robotModel::DeviceInfo>());

	MOCK_METHOD0(timeline, utils::TimelineInterface &());

	MOCK_METHOD0(onInterpretationStarted, void());
};

}
