#pragma once

#include <QtCore/QScopedPointer>

#include <gtest/gtest.h>

#include <interpreterCore/src/managers/devicesConfigurationManager.h>

#include "support/qrguiFacade.h"
#include "support/dummySensorsConfigurer.h"

namespace qrTest {
namespace robotsTests {
namespace interpreterCoreTests {

class SensorsConfigurationManagerTest : public testing::Test
{
protected:
	void SetUp() override;

	QScopedPointer<QrguiFacade> mQrguiFacade;
	QScopedPointer<interpreterCore::DevicesConfigurationManager> mManager;
	QScopedPointer<DummySensorsConfigurer> mConfigurer1;
	QScopedPointer<DummySensorsConfigurer> mConfigurer2;
};

}
}
}
