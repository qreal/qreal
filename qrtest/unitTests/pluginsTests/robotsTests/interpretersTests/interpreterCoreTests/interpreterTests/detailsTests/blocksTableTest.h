#pragma once

#include <gtest/gtest.h>

/// @todo /src/managers
#include <interpreterCore/managers/blocksFactoryManagerInterfaceMock.h>
#include <kitBase/robotModel/robotModelInterfaceMock.h>
#include <kitBase/robotModel/robotModelManagerInterfaceMock.h>

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
