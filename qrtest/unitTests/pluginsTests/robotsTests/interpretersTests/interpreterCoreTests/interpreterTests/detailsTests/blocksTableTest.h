#pragma once

#include <gtest/gtest.h>

/// @todo /src/managers
#include <interpreterCore/managers/blocksFactoryManagerInterfaceMock.h>
#include <interpreterBase/robotModel/robotModelInterfaceMock.h>
#include <interpreterBase/robotModel/robotModelManagerInterfaceMock.h>

namespace qrTest {
namespace robotsTests {
namespace interpreterCoreTests {
namespace detailsTests {

class BlocksTableTest : public testing::Test
{
protected:
	void SetUp() override;

	BlocksFactoryManagerInterfaceMock mBlocksFactoryManager;
	RobotModelInterfaceMock mModel;
	RobotModelManagerInterfaceMock mModelManager;
};

}
}
}
}
