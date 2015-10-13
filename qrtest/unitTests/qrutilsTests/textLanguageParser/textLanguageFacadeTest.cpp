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

#include "textLanguageFacadeTest.h"

#include "gtest/gtest.h"

#include "textLanguageParser/ast/nodes/integerNumber.h"
#include "textLanguageParser/types/integer.h"

using namespace textLanguageParser;
using namespace qrTest;

void TextLanguageFacadeTest::SetUp()
{
	mFacade.reset(new TextLanguageFacade());
}

TEST_F(TextLanguageFacadeTest, sanityCheck)
{
	qReal::Id const testId = qReal::Id("1", "2", "3", "test");
	int result = mFacade->interpret<int>(testId, "test", "123");
	EXPECT_EQ(123, result);
	EXPECT_TRUE(mFacade->errors().isEmpty());
	EXPECT_TRUE(mFacade->ast(testId, "test")->is<ast::IntegerNumber>());
	EXPECT_TRUE(mFacade->type(mFacade->ast(testId, "test"))->is<types::Integer>());
}
