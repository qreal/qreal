#include "kitPluginManagerTest.h"

#include <src/kitPluginManager.h>

using namespace qrTest::robotsInterpreterCoreTests;
using namespace robotsInterpreterCore;

TEST_F(KitPluginManagerTest, pluginsLoadingTest)
{
	KitPluginManager kitPluginManager("plugins/unittests");

	ASSERT_EQ(2, kitPluginManager.kitNames().size());
	ASSERT_EQ("MockKitPlugin1", kitPluginManager.kitNames()[0]);
	ASSERT_EQ("MockKitPlugin2", kitPluginManager.kitNames()[1]);
}
