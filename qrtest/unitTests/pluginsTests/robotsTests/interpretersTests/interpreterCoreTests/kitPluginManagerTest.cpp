/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "kitPluginManagerTest.h"

#include <interpreterCore/managers/kitPluginManager.h>

using namespace qrTest::robotsTests::interpreterCoreTests;
using namespace interpreterCore;

TEST_F(KitPluginManagerTest, pluginsLoadingTest)
{
	KitPluginManager kitPluginManager("plugins/unittests");

	ASSERT_EQ(2, kitPluginManager.kitIds().size());
	ASSERT_EQ("MockKitPlugin1", kitPluginManager.kitIds()[0]);
	ASSERT_EQ("MockKitPlugin2", kitPluginManager.kitIds()[1]);
}
