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

#include "settingsManagerTest.h"

using namespace qrTest;

void SettingsManagerTest::SetUp() {
	mSettingsManager = qReal::SettingsManager::instance();
	mDebugColor = mSettingsManager->value("debugColor").toString();
}

void SettingsManagerTest::TearDown() {
	mSettingsManager->setValue("debugColor", mDebugColor);
	mSettingsManager->saveData();
}

TEST_F(SettingsManagerTest, getSetTest) {
	mSettingsManager->setValue("debugColor", "test color");
	EXPECT_EQ(mSettingsManager->value("debugColor").toString(), "test color");
}

TEST_F(SettingsManagerTest, saveDataTest) {
	mSettingsManager->setValue("debugColor", "test color");

	mSettingsManager->load();

	EXPECT_EQ(mSettingsManager->value("debugColor").toString(), mDebugColor);

	mSettingsManager->setValue("debugColor", "test color");
	mSettingsManager->saveData();

	mSettingsManager->load();

	EXPECT_EQ(mSettingsManager->value("debugColor").toString(), "test color");
}

TEST_F(SettingsManagerTest, defaultValueTest) {
	QString const val = mSettingsManager->value("aabbccTestProperty", "default value").toString();
	EXPECT_EQ(val, "default value");
}
