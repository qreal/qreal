#pragma once

#include <../qrutils/expressionsParser/expressionsParser.h>
#include <mocks/qrgui/toolPluginInterface/usedInterface/errorReporterMock.h>

#include <gtest/gtest.h>

namespace qrTest {

class ExpressionsParserTest : public testing::Test {

protected:
	virtual void SetUp();

	virtual void TearDown();

	utils::ExpressionsParser *mParser;
	ErrorReporterMock mErrorReporter;
};

}
