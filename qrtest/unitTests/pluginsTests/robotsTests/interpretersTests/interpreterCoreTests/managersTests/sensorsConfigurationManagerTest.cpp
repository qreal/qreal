#include "sensorsConfigurationManagerTest.h"

using namespace qrTest::robotsTests::interpreterCoreTests;

using namespace interpreterCore;
using namespace interpreterBase::robotModel;
using namespace testing;

void SensorsConfigurationManagerTest::SetUp()
{
	mQrguiFacade.reset(new QrguiFacade("unittests/testModel.qrs"));

	mManager.reset(new SensorsConfigurationManager(mQrguiFacade->graphicalModelAssistInterface()
			, mQrguiFacade->logicalModelAssistInterface()
			, mQrguiFacade->mainWindowInterpretersInterface()
			, mQrguiFacade->systemEvents()
			));
}

TEST_F(SensorsConfigurationManagerTest, serializationTest)
{

}
