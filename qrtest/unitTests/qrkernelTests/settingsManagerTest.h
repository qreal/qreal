#pragma once

#include "../../../qrkernel/settingsManager.h"

#include "gtest/gtest.h"

namespace qrTest {

class SettingsManagerTest : public testing::Test {

protected:
	virtual void SetUp();

	virtual void TearDown();

	qReal::SettingsManager *mSettingsManager;
	QString mDebugColor;
};

}
