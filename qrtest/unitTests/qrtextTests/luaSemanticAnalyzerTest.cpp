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

#include "luaSemanticAnalyzerTest.h"

#include "qrtext/core/types/any.h"

#include "qrtext/lua/ast/assignment.h"
#include "qrtext/lua/ast/block.h"
#include "qrtext/lua/ast/functionCall.h"
#include "qrtext/lua/ast/indexingExpression.h"
#include "qrtext/lua/ast/unaryMinus.h"

#include "qrtext/lua/types/integer.h"
#include "qrtext/lua/types/float.h"
#include "qrtext/lua/types/string.h"
#include "qrtext/lua/types/table.h"

#include "gtest/gtest.h"

using namespace qrTest;
using namespace qrtext;
using namespace qrtext::lua;
using namespace qrtext::lua::details;
using namespace qrtext::lua::types;

void LuaSemanticAnalyzerTest::SetUp()
{
	mAnalyzer.reset(new LuaSemanticAnalyzer(mErrors));
	mParser.reset(new LuaParser(mErrors));
	mLexer.reset(new LuaLexer(mErrors));
}

QSharedPointer<qrtext::core::ast::Node> LuaSemanticAnalyzerTest::parse(const QString &code)
{
	return mParser->parse(mLexer->tokenize(code), mLexer->userFriendlyTokenNames());
}

TEST_F(LuaSemanticAnalyzerTest, sanityCheck)
{
	auto tree = parse("123");
	mAnalyzer->analyze(tree);
	EXPECT_TRUE(mErrors.empty());
	EXPECT_TRUE(mAnalyzer->type(tree)->is<Integer>());
	EXPECT_TRUE(mAnalyzer->type(tree)->is<Number>());
}

TEST_F(LuaSemanticAnalyzerTest, assignment)
{
	auto tree = parse("a = 123");

	auto variable = as<ast::Assignment>(tree)->variable();
	auto value = as<ast::Assignment>(tree)->value();

	mAnalyzer->analyze(tree);

	EXPECT_TRUE(mErrors.empty());
	EXPECT_TRUE(mAnalyzer->type(variable)->is<types::Integer>());
	EXPECT_TRUE(mAnalyzer->type(value)->is<types::Integer>());
}

TEST_F(LuaSemanticAnalyzerTest, unaryOperator)
{
	auto tree = parse("-123");

	mAnalyzer->analyze(tree);

	EXPECT_TRUE(mErrors.empty());
	EXPECT_TRUE(mAnalyzer->type(as<ast::UnaryMinus>(tree))->is<types::Integer>());
}

TEST_F(LuaSemanticAnalyzerTest, propagation)
{
	auto tree = parse("a = -123; b = a");

	mAnalyzer->analyze(tree);

	EXPECT_TRUE(mErrors.empty());

	auto block = as<ast::Block>(tree);
	auto firstAssignment = as<ast::Assignment>(block->children()[0]);
	auto secondAssignment = as<ast::Assignment>(block->children()[1]);

	auto firstVariable = firstAssignment->variable();
	auto firstValue = firstAssignment->value();

	EXPECT_TRUE(mAnalyzer->type(firstVariable)->is<types::Integer>());
	EXPECT_TRUE(mAnalyzer->type(firstValue)->is<types::Integer>());

	auto secondVariable = secondAssignment->variable();
	auto secondValue = secondAssignment->value();

	EXPECT_TRUE(mAnalyzer->type(secondVariable)->is<types::Integer>());
	EXPECT_TRUE(mAnalyzer->type(secondValue)->is<types::Integer>());
}

TEST_F(LuaSemanticAnalyzerTest, integerFloatCoercion)
{
	auto tree = parse("a = -123; a = 1.0");

	mAnalyzer->analyze(tree);

	EXPECT_TRUE(mErrors.isEmpty());

	auto block = as<ast::Block>(tree);
	auto firstAssignment = as<ast::Assignment>(block->children()[0]);
	auto secondAssignment = as<ast::Assignment>(block->children()[1]);

	auto firstVariable = firstAssignment->variable();
	auto firstValue = firstAssignment->value();

	EXPECT_TRUE(mAnalyzer->type(firstVariable)->is<types::Float>());
	EXPECT_TRUE(mAnalyzer->type(firstValue)->is<types::Integer>());

	auto secondVariable = secondAssignment->variable();
	auto secondValue = secondAssignment->value();

	EXPECT_TRUE(mAnalyzer->type(secondVariable)->is<types::Float>());
	EXPECT_TRUE(mAnalyzer->type(secondValue)->is<types::Float>());
}

TEST_F(LuaSemanticAnalyzerTest, functionReturnType)
{
	auto tree = parse("a = f(1)");

	mAnalyzer->addIntrinsicFunction("f", QSharedPointer<types::Function>(new types::Function(
			QSharedPointer<core::types::TypeExpression>(new types::Float()),
			{QSharedPointer<core::types::TypeExpression>(new types::Integer())}
			)));

	mAnalyzer->analyze(tree);

	EXPECT_TRUE(mErrors.isEmpty());

	auto assignment = as<ast::Assignment>(tree);

	auto a = assignment->variable();
	auto f = assignment->value();

	EXPECT_TRUE(mAnalyzer->type(a)->is<types::Float>());
	EXPECT_TRUE(mAnalyzer->type(f)->is<types::Float>());
}

TEST_F(LuaSemanticAnalyzerTest, functionParameters)
{
	auto tree = parse("b = 1; c = 'c'; a = f(b, c)");

	mAnalyzer->addIntrinsicFunction("f", QSharedPointer<types::Function>(new types::Function(
			QSharedPointer<core::types::TypeExpression>(new types::Float()),
			{QSharedPointer<core::types::TypeExpression>(new types::Integer())
					, QSharedPointer<core::types::TypeExpression>(new types::String())}
			)));

	mAnalyzer->analyze(tree);

	EXPECT_TRUE(mErrors.isEmpty());

	auto assignment = as<ast::Assignment>(as<ast::Block>(tree)->children()[2]);

	auto f = as<ast::FunctionCall>(assignment->value());

	auto b = f->arguments()[0];
	auto c = f->arguments()[1];

	EXPECT_TRUE(mAnalyzer->type(b)->is<types::Integer>());
	EXPECT_TRUE(mAnalyzer->type(c)->is<types::String>());
}

TEST_F(LuaSemanticAnalyzerTest, invalidFunctionParameters)
{
	auto tree = parse("a = f(0.5, 'a')");

	mAnalyzer->addIntrinsicFunction("f", QSharedPointer<types::Function>(new types::Function(
			QSharedPointer<core::types::TypeExpression>(new types::Float()),
			{QSharedPointer<core::types::TypeExpression>(new types::Integer())
					, QSharedPointer<core::types::TypeExpression>(new types::String())}
			)));

	mAnalyzer->analyze(tree);

	ASSERT_FALSE(mErrors.isEmpty());
}

TEST_F(LuaSemanticAnalyzerTest, invalidCoercion)
{
	auto tree = parse("a = f(b, c); b = 0.5");

	mAnalyzer->addIntrinsicFunction("f", QSharedPointer<types::Function>(new types::Function(
			QSharedPointer<core::types::TypeExpression>(new types::Float()),
			{QSharedPointer<core::types::TypeExpression>(new types::Integer())
					, QSharedPointer<core::types::TypeExpression>(new types::String())}
			)));

	mAnalyzer->analyze(tree);

	EXPECT_FALSE(mErrors.isEmpty());

	auto block = as<ast::Block>(tree);
	auto firstAssignment = as<ast::Assignment>(block->children()[0]);

	auto a = firstAssignment->variable();
	auto f = as<ast::FunctionCall>(firstAssignment->value());

	EXPECT_TRUE(mAnalyzer->type(a)->is<types::Float>());

	auto b = f->arguments()[0];
	auto c = f->arguments()[1];

	EXPECT_TRUE(mAnalyzer->type(b)->is<qrtext::core::types::Any>());
	EXPECT_TRUE(mAnalyzer->type(c)->is<types::String>());
}

TEST_F(LuaSemanticAnalyzerTest, tableIndexingExpression)
{
	auto tree = parse("a[1] = 1");

	mAnalyzer->analyze(tree);

	EXPECT_TRUE(mErrors.isEmpty());

	auto assignment = as<ast::Assignment>(tree);

	auto indexingExpression = as<ast::IndexingExpression>(assignment->variable());
	auto value = assignment->value();

	auto table = indexingExpression->table();
	auto index = indexingExpression->indexer();

	EXPECT_TRUE(mAnalyzer->type(value)->is<types::Integer>());
	EXPECT_TRUE(mAnalyzer->type(indexingExpression)->is<types::Integer>());
	EXPECT_TRUE(mAnalyzer->type(table)->is<types::Table>());
	EXPECT_TRUE(mAnalyzer->type(index)->is<types::Integer>());

	tree = parse("a[1]");

	mAnalyzer->analyze(tree);

	indexingExpression = as<ast::IndexingExpression>(tree);

	table = indexingExpression->table();
	index = indexingExpression->indexer();

	EXPECT_TRUE(mAnalyzer->type(indexingExpression)->is<types::Integer>());
	EXPECT_TRUE(mAnalyzer->type(table)->is<types::Table>());
	EXPECT_TRUE(mAnalyzer->type(index)->is<types::Integer>());
}

TEST_F(LuaSemanticAnalyzerTest, complexByteOperationsExpression)
{
	auto tree = parse("a2 = 7 | 15 & 40");

	mAnalyzer->analyze(tree);

	EXPECT_TRUE(mErrors.isEmpty());

	auto assignment = as<ast::Assignment>(tree);

	auto a2 = assignment->variable();

	EXPECT_TRUE(mAnalyzer->type(a2)->is<types::Integer>());

	mAnalyzer->clear();

	tree = parse("a2 = 3 + 7 | 15 & 40");

	mAnalyzer->analyze(tree);

	EXPECT_TRUE(mErrors.isEmpty());

	assignment = as<ast::Assignment>(tree);
	a2 = assignment->variable();

	EXPECT_TRUE(mAnalyzer->type(a2)->is<types::Integer>());
}

TEST_F(LuaSemanticAnalyzerTest, unknownIdentifier)
{
	auto tree = parse("a = b");

	mAnalyzer->analyze(tree);

	EXPECT_EQ(1, mErrors.size());
}

TEST_F(LuaSemanticAnalyzerTest, subarrayAssignment)
{
	auto tree = parse("a = {{1}}");
	mAnalyzer->analyze(tree);
	tree = parse("a[0] = {2}");
	mAnalyzer->analyze(tree);

	EXPECT_EQ(0, mErrors.size());
}

TEST_F(LuaSemanticAnalyzerTest, incorrectTableElementType)
{
	auto tree = parse("a = {1}; a[0] = 1.1");
	mAnalyzer->analyze(tree);

	EXPECT_FALSE(mErrors.isEmpty());
}
