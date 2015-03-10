#pragma once

#include <kitBase/robotModel/robotModelManagerInterface.h>
#include <kitBase/robotModel/configuration.h>

#include <gmock/gmock.h>

namespace qrTest {

class RobotModelManagerInterfaceMock : public  kitBase::robotModel::RobotModelManagerInterface
{
	Q_OBJECT

public:
	MOCK_CONST_METHOD0(model,  kitBase::robotModel::RobotModelInterface &());

	void emitConnected() {
		emit connected(true, "");
	}

	void emitAllDevicesConfigured() {
		emit allDevicesConfigured();
	}

	void emitDisconnected() {
		emit disconnected();
	}
};

}
