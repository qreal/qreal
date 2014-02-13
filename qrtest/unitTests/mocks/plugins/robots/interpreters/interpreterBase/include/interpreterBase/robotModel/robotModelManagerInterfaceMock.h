#pragma once

#include <interpreterBase/robotModel/robotModelManagerInterface.h>
#include <interpreterBase/robotModel/configuration.h>

#include <gmock/gmock.h>

namespace qrTest {

class RobotModelManagerInterfaceMock : public interpreterBase::robotModel::RobotModelManagerInterface
{
	Q_OBJECT

public:
	MOCK_CONST_METHOD0(model, interpreterBase::robotModel::RobotModelInterface &());

	void emitConnected() {
		emit connected(true);
	}

	void emitAllDevicesConfigured() {
		emit allDevicesConfigured();
	}

	void emitDisconnected() {
		emit disconnected();
	}
};

}
