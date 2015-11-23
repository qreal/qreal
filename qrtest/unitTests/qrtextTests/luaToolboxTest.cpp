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

#include "luaToolboxTest.h"

#include "gtest/gtest.h"

#include "qrtext/lua/ast/integerNumber.h"
#include "qrtext/lua/types/integer.h"
#include "qrtext/lua/types/float.h"

using namespace qrtext::lua;
using namespace qrTest;

void LuaToolboxTest::SetUp()
{
	mToolbox.reset(new LuaToolbox());
}

TEST_F(LuaToolboxTest, sanityCheck)
{
	qReal::Id const testId = qReal::Id("1", "2", "3", "test");
	int result = mToolbox->interpret<int>(testId, "test", "123");
	EXPECT_EQ(123, result);
	EXPECT_TRUE(mToolbox->errors().isEmpty());
	EXPECT_TRUE(mToolbox->ast(testId, "test")->is<ast::IntegerNumber>());
	EXPECT_TRUE(mToolbox->type(mToolbox->ast(testId, "test"))->is<types::Integer>());
}

TEST_F(LuaToolboxTest, easyTableInitializationSyntax)
{
	qReal::Id const testId = qReal::Id("1", "2", "3", "test");
	QStringList result = mToolbox->interpret<QStringList>(testId, "test", "'M1', 'M2', 'M3'");
	ASSERT_EQ(3, result.size());
	EXPECT_TRUE(mToolbox->errors().isEmpty());

	EXPECT_EQ("M1", result[0]);
	EXPECT_EQ("M2", result[1]);
	EXPECT_EQ("M3", result[2]);
}

TEST_F(LuaToolboxTest, intrinsicFunction)
{
	mToolbox->addIntrinsicFunction("f", new types::Integer(), {new types::Integer()}
			, [] (QList<QVariant> params) { return params[0].toInt() + 2; }
			);

	qReal::Id const testId = qReal::Id("1", "2", "3", "test");

	auto result = mToolbox->interpret<int>(testId, "test", "f(1)");
	EXPECT_TRUE(mToolbox->errors().isEmpty());

	EXPECT_EQ(3, result);
}

TEST_F(LuaToolboxTest, intrinsicFunctionAsIdentifier)
{
	mToolbox->addIntrinsicFunction("f", new types::Integer(), {new types::Integer()}
			, [] (QList<QVariant> params) { return params[0].toInt() + 2; }
			);

	qReal::Id const testId = qReal::Id("1", "2", "3", "test");

	mToolbox->interpret<int>(testId, "test", "f = 1");

	EXPECT_EQ(2, mToolbox->errors().size());
}

TEST_F(LuaToolboxTest, errorProcessingSanityCheck)
{
	mToolbox->interpret<int>("true ||| false");
	EXPECT_FALSE(mToolbox->errors().isEmpty());
}

TEST_F(LuaToolboxTest, tables)
{
	mToolbox->interpret<int>("a = {10; 15}");

	int result = mToolbox->interpret<int>("a[0]");
	EXPECT_TRUE(mToolbox->errors().isEmpty());
	EXPECT_EQ(10, result);

	result = mToolbox->interpret<int>("a[1]");
	EXPECT_TRUE(mToolbox->errors().isEmpty());
	EXPECT_EQ(15, result);
}

TEST_F(LuaToolboxTest, concatenation)
{
	mToolbox->interpret<int>("s = \"a = \" .. \"1\"");
	auto result = mToolbox->interpret<QString>("s");
	ASSERT_TRUE(mToolbox->errors().isEmpty());
	EXPECT_EQ("a = 1", result);
}

TEST_F(LuaToolboxTest, memory)
{
	ASSERT_EQ(0, qrtext::core::ast::Node::nodesCount());

	mToolbox->interpret<int>("sensorA1 = 30; M4 = 'M4'; M3 = 'M3'");

	mToolbox->interpret<int>("S=sensorA1; Sold=S;");
	mToolbox->interpret<int>("u=2.5*(S-sensorA1)+5*(Sold-sensorA1); Sold=sensorA1;");
	mToolbox->interpret<int>("M4");
	mToolbox->interpret<int>("50-u");
	mToolbox->interpret<int>("M3");
	mToolbox->interpret<int>("50+u");
	mToolbox->interpret<int>("30");

	EXPECT_NE(0, qrtext::core::ast::Node::nodesCount());

	mToolbox.reset(new LuaToolbox());

	EXPECT_EQ(0, qrtext::core::ast::Node::nodesCount());

	mToolbox->interpret<int>(qReal::Id("1", "2", "3", "test"), "test", "123");

	EXPECT_NE(0, qrtext::core::ast::Node::nodesCount());

	mToolbox.reset();

	EXPECT_EQ(0, qrtext::core::ast::Node::nodesCount());
}

TEST_F(LuaToolboxTest, integerDivision)
{
	mToolbox->interpret<int>("s = 5 // 2");
	auto result = mToolbox->interpret<int>("s");
	ASSERT_TRUE(mToolbox->errors().isEmpty());
	EXPECT_EQ(2, result);
}

TEST_F(LuaToolboxTest, twoDTables)
{
	mToolbox->interpret<int>("a = {{1}}");
	int result = mToolbox->interpret<int>("a[0][0]");
	ASSERT_TRUE(mToolbox->errors().isEmpty());
	EXPECT_EQ(1, result);
}

TEST_F(LuaToolboxTest, twoFunctions)
{
	const auto add1aryFunction = [this] (const QString &name
			, qrtext::core::types::TypeExpression * const returnType
			, qrtext::core::types::TypeExpression * const argumentType
			, std::function<QVariant(QVariant)> const &function)
	{
		mToolbox->addIntrinsicFunction(name, returnType
				, {argumentType}
				, [function] (const QList<QVariant> &params) {
						Q_ASSERT(!params.isEmpty());
						return function(params.first());
				});
	};

	const auto addFloatFunction = [this, add1aryFunction] (const QString &name
			, std::function<qreal(qreal)> const &function)
	{
		add1aryFunction(name, new types::Float, new types::Float
				, [function](const QVariant &arg) { return function(arg.toReal()); });
	};

	addFloatFunction("cos", [](qreal x) {return cos(x); });

	mToolbox->interpret<int>("cos(1)");
	mToolbox->interpret<int>("cos(1)");
	ASSERT_TRUE(mToolbox->errors().isEmpty());
}
