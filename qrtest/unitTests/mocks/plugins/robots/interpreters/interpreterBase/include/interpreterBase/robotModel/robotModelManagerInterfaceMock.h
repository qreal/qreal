#pragma once

#include <interpreterBase/robotModel/robotModelManagerInterface.h>
#include <interpreterBase/robotModel/configuration.h>

#include <gmock/gmock.h>

namespace qrTest {

class RobotModelManagerInterfaceMock : public interpreterBase::robotModel::RobotModelManagerInterface
{
public:
	MOCK_CONST_METHOD0(model, interpreterBase::robotModel::RobotModelInterface &());
};

}
