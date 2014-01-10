#include "interpreterTest.h"

#include <src/interpreter/interpreter.h>

using namespace qrTest::robotsTests::interpreterCoreTests;

using namespace interpreterCore::interpreter;
using namespace ::testing;

void InterpreterTest::SetUp()
{
	mBlocksFactory = new DummyBlockFactory();

	mQrguiFacade = new QrguiFacade("unittests/testModel.qrs");

	ON_CALL(mModel, needsConnection()).WillByDefault(Return(false));

	ON_CALL(mModel, init()).WillByDefault(Invoke([&] {
		QMetaObject::invokeMethod(&mModel, "connected", Q_ARG(bool, true));
		QMetaObject::invokeMethod(&mModel, "sensorsConfigured");
	}));

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
