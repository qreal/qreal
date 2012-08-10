#pragma once

#include "../../plugins/blockDiagram/visualDebugSupport/interpreter/blockParser.h"
#include "gtest/gtest.h"

class BlockParserExampleTest : public testing::Test {

protected:
	virtual void SetUp();

	virtual void TearDown();

	qReal::BlockParser *blockParser;
};
