#pragma once

#include <interpreterCore/managers/blocksFactoryManagerInterface.h>

#include <gmock/gmock.h>

namespace qrTest {

class BlocksFactoryManagerInterfaceMock : public interpreterCore::BlocksFactoryManagerInterface
{
public:
	MOCK_METHOD2(addFactory, void(interpreterBase::blocksBase::BlocksFactoryInterface * const factory
			, interpreterBase::robotModel::RobotModelInterface const *robotModel));
	MOCK_METHOD2(block, qReal::interpretation::BlockInterface *(qReal::Id const &element
			, interpreterBase::robotModel::RobotModelInterface const &robotModel));
	MOCK_CONST_METHOD1(enabledBlocks, QSet<qReal::Id>(
			interpreterBase::robotModel::RobotModelInterface const &robotModel));
	MOCK_CONST_METHOD1(visibleBlocks, QSet<qReal::Id>(
			interpreterBase::robotModel::RobotModelInterface const &robotModel));
	MOCK_CONST_METHOD0(commonBlocks, QSet<qReal::Id>());
};

}
