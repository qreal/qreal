#include "interpreterTest.h"

#include <src/interpreter/interpreter.h>

#include <qrgui/models/models.h>
#include <qrgui/pluginManager/editorManager.h>
#include <mocks/grgui/mainwindow/mainWindowInterpretersInterfaceMock.h>

using namespace qrTest::robotsInterpreterCoreTests;

using namespace robotsInterpreterCore::interpreter;

void InterpreterTest::SetUp()
{
	qReal::EditorManager editorManager;

	qReal::models::Models models("unittests/testModel.qrs", editorManager);
	qrTest::MainWindowInterpretersInterfaceMock mainWindowInterpretersInterfaceMock;

	mInterpreter = new Interpreter(
			models.graphicalModelAssistApi()
			, models.logicalModelAssistApi()
			, mainWindowInterpretersInterfaceMock
			);
}

void InterpreterTest::TearDown()
{
}
