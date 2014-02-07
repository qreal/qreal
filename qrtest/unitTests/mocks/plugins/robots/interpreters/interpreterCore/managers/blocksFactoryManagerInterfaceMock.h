#pragma once

#include <../interpreterCore/src/managers/blocksFactoryManagerInterface.h>

#include <gmock/gmock.h>

namespace qrTest {

class BlocksFactoryManagerInterfaceMock : public interpreterCore::BlocksFactoryManagerInterface
{
public:
	MOCK_METHOD1(addFactory, void(interpreterBase::blocksBase::BlocksFactoryInterface * const factory));
	MOCK_METHOD1(block, interpreterBase::blocksBase::BlockInterface *(qReal::Id const &element));
	MOCK_CONST_METHOD0(providedBlocks, qReal::IdList());
};

}
