/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "sampleFixtureTest.h"

void BlockParserExampleTest::SetUp() {
	blockParser = new qReal::BlockParser(NULL);
}

void BlockParserExampleTest::TearDown() {
	delete blockParser;
}

TEST_F(BlockParserExampleTest, expression) {
	QString const stream = "(2+3)*5";
	int pos = 0;
	EXPECT_EQ(blockParser->parseExpression(stream, pos)->value().toInt(), 25);
}

TEST_F(BlockParserExampleTest, condition) {
	QString const stream = "2 + 3 < 5 * 5";
	int pos = 0;
	ASSERT_TRUE(blockParser->parseConditionHelper(stream, pos));
}
