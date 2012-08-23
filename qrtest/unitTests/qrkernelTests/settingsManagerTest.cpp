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
