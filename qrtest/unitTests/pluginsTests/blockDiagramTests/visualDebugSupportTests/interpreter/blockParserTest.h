#pragma once

#include <plugins/blockDiagram/visualDebugSupport/interpreter/blockParser.h>
#include <mocks/qrgui/plugins/toolPluginInterface/usedInterface/errorReporterMock.h>

#include <gtest/gtest.h>

namespace qrTest {

class BlockParserTest : public testing::Test {

protected:
	virtual void SetUp();

	virtual void TearDown();

	qReal::BlockParser *mParser;
	ErrorReporterMock mErrorReporter;
};

}
