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

#include "../../../qrutils/expressionsParser/number.h"
#include "../../../plugins/blockDiagram/visualDebugSupport/interpreter/blockParser.h"

#include "gtest/gtest.h"

using namespace utils;

TEST(NumberExampleTest, sum) {
	Number n1(2, Number::intType);
	Number n2(3, Number::intType);
	n1 += n2;
	EXPECT_EQ(n1.value().toInt(), 5);
}

TEST(ParserExampleTest, expressionCalculation) {
	qReal::BlockParser *parser = new qReal::BlockParser(NULL);
	QString const stream = "1+1";
	int pos = 0;
	EXPECT_EQ(parser->parseExpression(stream, pos)->value().toInt(), 2);
}
