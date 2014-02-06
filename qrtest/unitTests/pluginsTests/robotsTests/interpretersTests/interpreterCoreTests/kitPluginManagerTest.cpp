#include "kitPluginManagerTest.h"

#include <src/managers/kitPluginManager.h>

using namespace qrTest::robotsTests::interpreterCoreTests;
using namespace interpreterCore;

TEST_F(KitPluginManagerTest, pluginsLoadingTest)
{
	KitPluginManager kitPluginManager("plugins/unittests");

	ASSERT_EQ(2, kitPluginManager.kitIds().size());
	ASSERT_EQ("MockKitPlugin1", kitPluginManager.kitIds()[0]);
	ASSERT_EQ("MockKitPlugin2", kitPluginManager.kitIds()[1]);
}
