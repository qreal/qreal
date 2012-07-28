#pragma once

#include "../plugins/blockDiagram/visualDebugSupport/interpreter/blockParser.h"
#include "gtest/gtest.h"

class BlockParserTest : public testing::Test {

protected:
	virtual void SetUp();

	virtual void TearDown();

	qReal::BlockParser *blockParser;
};
