#pragma once

#include "../../../qrutils/expressionsParser/expressionsParser.h"
#include "../mocks/grgui/toolPluginInterface/usedInterface/errorReporterMock.h"

#include "gtest/gtest.h"

namespace qrTest {

class ExpressionParserTest : public testing::Test {

protected:
	virtual void SetUp();

	virtual void TearDown();

	utils::ExpressionsParser *mParser;
	ErrorReporterMock mErrorReporter;
};

}
