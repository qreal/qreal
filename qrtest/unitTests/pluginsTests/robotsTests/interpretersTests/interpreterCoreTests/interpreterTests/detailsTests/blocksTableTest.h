#pragma once

#include <gtest/gtest.h>

/// @todo /src/managers
#include <interpreterCore/managers/blocksFactoryManagerInterfaceMock.h>

namespace qrTest {
namespace robotsTests {
namespace interpreterCoreTests {
namespace detailsTests {

class BlocksTableTest : public testing::Test
{
protected:
	void SetUp() override;

	qrTest::BlocksFactoryManagerInterfaceMock mBlocksFactoryManager;
};

}
}
}
}
