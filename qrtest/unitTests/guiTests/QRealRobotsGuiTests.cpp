/* Copyright 2015 QReal Research Group
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

#include "QRealRobotsGuiTests.h"

#include <QDebug>

using namespace guiTesting;
using namespace qReal;

TEST_F(QRealRobotsGuiTests, sanityCheck)
{
	run("assert(true);");
	ASSERT_EQ(2, 1 + 1);
}

TEST_F(QRealRobotsGuiTests, editActionsExistence)
{
	runFromFile("editActionsExistence.js"); // мб qs?
}

TEST_F(QRealRobotsGuiTests, viewActionsExistence)
{
	runFromFile("viewActionsExistence.js");
}

TEST_F(QRealRobotsGuiTests, findDialogElementsExistence)
{
	runFromFile("findDialogElementsExistence.js");
}

TEST_F(QRealRobotsGuiTests, helpActionsExistence)
{
	runFromFile("helpActionsExistence.js");
}

TEST_F(QRealRobotsGuiTests, mainPanelsElementsExistence)
{
	runFromFile("mainPanelsElementsExistence.js");
}

TEST_F(QRealRobotsGuiTests, mouseGesturesElementsExistence)
{
	runFromFile("mouseGesturesElementsExistence.js");
}

TEST_F(QRealRobotsGuiTests, preferenceDialogElementsExistence)
{
	runFromFile("preferenceDialogElementsExistence.js");
}

TEST_F(QRealRobotsGuiTests, settingsActionsExistence)
{
	runFromFile("settingsActionsExistence.js");
}

TEST_F(QRealRobotsGuiTests, tabSceneExistence)
{
	runFromFile("tabSceneExistence.js");
}

TEST_F(QRealRobotsGuiTests, toolbarsElementsExistence)
{
	runFromFile("toolbarsElementsExistence.js");
}

TEST_F(QRealRobotsGuiTests, toolsActionsExistence)
{
	runFromFile("toolsActionsExistence.js");
}

TEST_F(QRealRobotsGuiTests, fileActionsExistence)
{
	runFromFile("fileActionsExistence.js");
}

TEST_F(QRealRobotsGuiTests, fileActionsFunctioning)
{
	runFromFile("fileActionsFunctioning.js");
}

TEST_F(QRealRobotsGuiTests, findDialogElementsFunctioning)
{
	runFromFile("findDialogElementsFunctioning.js");
}

TEST_F(QRealRobotsGuiTests, helpActionsFunctioning)
{
	runFromFile("helpActionsFunctioning.js");
}

TEST_F(QRealRobotsGuiTests, mainPanelsElementsFunctioning)
{
	runFromFile("mainPanelsElementsFunctioning.js");
}

TEST_F(QRealRobotsGuiTests, mouseGesturesElementsFunctioning)
{
	runFromFile("mouseGesturesElementsFunctioning.js");
}

TEST_F(QRealRobotsGuiTests, preferenceDialogElementsFunctioning)
{
	runFromFile("preferenceDialogElementsFunctioning.js");
}

TEST_F(QRealRobotsGuiTests, settingsActionsFunctioning)
{
	runFromFile("settingsActionsFunctioning.js");
}

TEST_F(QRealRobotsGuiTests, tabSceneFunctioning)
{
	runFromFile("tabSceneFunctioning.js");
}

TEST_F(QRealRobotsGuiTests, toolbarsElementsFunctioning)
{
	runFromFile("toolbarsElementsFunctioning.js");
}

TEST_F(QRealRobotsGuiTests, toolsActionsFunctioning)
{
	runFromFile("toolsActionsFunctioning.js");
}

TEST_F(QRealRobotsGuiTests, viewActionsFunctioning)
{
	runFromFile("viewActionsFunctioning.js");
}

TEST_F(QRealRobotsGuiTests, editActionsFunctioning)
{
	runFromFile("editActionsFunctioning.js");
}

TEST_F(QRealRobotsGuiTests, hotKeysExistanceAndFunctioning)
{
	runFromFile("hotKeysExistanceAndFunctioning.js");
}

TEST_F(QRealRobotsGuiTests, script1)
{
	runFromFile("script1.js");
}

TEST_F(QRealRobotsGuiTests, script2)
{
	runFromFile("script2.js");
}

TEST_F(QRealRobotsGuiTests, script3)
{
	runFromFile("script3.js");
}

TEST_F(QRealRobotsGuiTests, script4)
{
	runFromFile("script4.js");
}

TEST_F(QRealRobotsGuiTests, script5)
{
	runFromFile("script5.js");
}

TEST_F(QRealRobotsGuiTests, script6)
{
	runFromFile("script6.js");
}

TEST_F(QRealRobotsGuiTests, script7)
{
	runFromFile("script7.js");
}

TEST_F(QRealRobotsGuiTests, script8)
{
	runFromFile("script8.js");
}

TEST_F(QRealRobotsGuiTests, script9)
{
	runFromFile("script9.js");
}

TEST_F(QRealRobotsGuiTests, script10)
{
	runFromFile("script10.js");
}
