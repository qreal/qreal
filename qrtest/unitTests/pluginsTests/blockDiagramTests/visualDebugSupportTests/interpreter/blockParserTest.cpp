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

#include "blockParserTest.h"

#include "gtest/gtest.h"

using namespace utils;
using namespace qReal;
using namespace qrTest;

using ::testing::Exactly;
using ::testing::_;

void BlockParserTest::SetUp() {
	mParser = new BlockParser(&mErrorReporter);
}

void BlockParserTest::TearDown() {
	delete mParser;
}

TEST_F(BlockParserTest, parseProcessTest) {
	QString const stream = "var int a, b; double c; a=5; b=3; c=a+b;";
	int pos = 0;

	mParser->parseProcess(stream, pos, Id::rootId());

	QMap<QString, Number *> const &variables = mParser->variables();
	QStringList const variableNames = variables.keys();

	ASSERT_EQ(variableNames.size(), 3);
	ASSERT_TRUE(variableNames.contains("a"));
	ASSERT_TRUE(variableNames.contains("b"));
	ASSERT_TRUE(variableNames.contains("c"));

	Number const *a = variables.value("a");
	Number const *b = variables.value("b");
	Number const *c = variables.value("c");

	EXPECT_EQ(a->value().toInt(), 5);
	EXPECT_EQ(b->value().toInt(), 3);
	EXPECT_EQ(c->value().toDouble(), 8);
}
