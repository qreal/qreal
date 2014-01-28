#include "interpreterTest.h"

#include <src/interpreter/interpreter.h>

#include <interpreterBase/robotModel/configurationInterfaceMock.h>

using namespace qrTest::robotsTests::interpreterCoreTests;

using namespace interpreterCore::interpreter;
using namespace ::testing;

void InterpreterTest::SetUp()
{
	mBlocksFactory = new DummyBlockFactory();

	mQrguiFacade = new QrguiFacade("unittests/testModel.qrs");

	ConfigurationInterfaceMock configurationInterfaceMock;

	ON_CALL(mModel, needsConnection()).WillByDefault(Return(false));

	ON_CALL(mModel, init()).WillByDefault(Invoke([&] {
		QMetaObject::invokeMethod(&mModel, "connected", Q_ARG(bool, true));
		QMetaObject::invokeMethod(&mModel.configuration(), "allDevicesConfigured");
	}));

	ON_CALL(mModel, configuration()).WillByDefault(ReturnRef(configurationInterfaceMock));

	EXPECT_CALL(mModel, configuration()).Times(AtLeast(1));
	EXPECT_CALL(mModel, needsConnection()).Times(AtLeast(1));
	EXPECT_CALL(mModel, init()).Times(AtLeast(1));

	mInterpreter = new Interpreter(
			mQrguiFacade->graphicalModelAssistInterface()
			, mQrguiFacade->logicalModelAssistInterface()
			, mQrguiFacade->mainWindowInterpretersInterface()
			, mQrguiFacade->projectManagementInterface()
			, mBlocksFactory
			, &mModel
			);
}

void InterpreterTest::TearDown()
{
	delete mInterpreter;
	delete mQrguiFacade;
}

TEST_F(InterpreterTest, sanityCheck)
{
}

TEST_F(InterpreterTest, interpret)
{
	mInterpreter->interpret();
}
