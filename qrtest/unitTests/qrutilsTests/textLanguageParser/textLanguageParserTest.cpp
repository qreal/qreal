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

#include "textLanguageParserTest.h"

#include "textLanguageParser/ast/nodes/number.h"
#include "textLanguageParser/ast/nodes/integerNumber.h"
#include "textLanguageParser/ast/nodes/floatNumber.h"
#include "textLanguageParser/ast/nodes/unaryOperator.h"
#include "textLanguageParser/ast/nodes/binaryOperator.h"

#include "textLanguageParser/ast/nodes/functionCall.h"
#include "textLanguageParser/ast/nodes/methodCall.h"
#include "textLanguageParser/ast/nodes/indexingExpression.h"
#include "textLanguageParser/ast/nodes/block.h"
#include "textLanguageParser/ast/nodes/assignment.h"

#include "textLanguageParser/ast/nodes/addition.h"
#include "textLanguageParser/ast/nodes/multiplication.h"
#include "textLanguageParser/ast/nodes/exponentiation.h"

#include "textLanguageParser/ast/nodes/tableConstructor.h"

#include "textLanguageParser/ast/nodes/identifier.h"
#include "textLanguageParser/ast/nodes/string.h"

#include "gtest/gtest.h"

using namespace textLanguageParser;
using namespace qrTest;

void TextLanguageParserTest::SetUp()
{
	mParser.reset(new textLanguageParser::TextLanguageParser());
}

TEST_F(TextLanguageParserTest, sanityCheck)
{
	QString const stream = "123";
	auto result = mParser->parse(stream);
	EXPECT_TRUE(result.errors.isEmpty());
	auto number = result.astRoot.dynamicCast<ast::Number>();
	ASSERT_FALSE(number.isNull());
	EXPECT_EQ("123", number->stringRepresentation());
}

TEST_F(TextLanguageParserTest, unaryOp)
{
	QString const stream = "-123";
	auto result = mParser->parse(stream);
	EXPECT_TRUE(result.errors.isEmpty());
	QSharedPointer<ast::UnaryOperator> unaryOp = result.astRoot.dynamicCast<ast::UnaryOperator>();
	ASSERT_FALSE(unaryOp.isNull());
	QSharedPointer<ast::Number> operand = unaryOp->operand().dynamicCast<ast::Number>();
	ASSERT_FALSE(operand.isNull());
	EXPECT_EQ("123", operand->stringRepresentation());
}

TEST_F(TextLanguageParserTest, connections)
{
	QString const stream = "-123";
	auto result = mParser->parse(stream);
	EXPECT_TRUE(result.errors.isEmpty());
	QSharedPointer<ast::UnaryOperator> unaryOp = result.astRoot.dynamicCast<ast::UnaryOperator>();
	ASSERT_FALSE(unaryOp.isNull());
	EXPECT_EQ(ast::Connection(0, 0, 0), unaryOp->start());
	QSharedPointer<ast::Number> operand = unaryOp->operand().dynamicCast<ast::Number>();
	ASSERT_FALSE(operand.isNull());
	EXPECT_EQ(ast::Connection(1, 0, 1), operand->start());
	EXPECT_EQ(ast::Connection(3, 0, 3), operand->end());
}

TEST_F(TextLanguageParserTest, binaryOp)
{
	QString const stream = "1+2";
	auto result = mParser->parse(stream);
	EXPECT_TRUE(result.errors.isEmpty());
	QSharedPointer<ast::BinaryOperator> binaryOp = result.astRoot.dynamicCast<ast::BinaryOperator>();
	ASSERT_FALSE(binaryOp.isNull());
	QSharedPointer<ast::Number> leftOperand = binaryOp->leftOperand().dynamicCast<ast::Number>();
	ASSERT_FALSE(leftOperand.isNull());
	EXPECT_EQ("1", leftOperand->stringRepresentation());
	QSharedPointer<ast::Number> rightOperand = binaryOp->rightOperand().dynamicCast<ast::Number>();
	ASSERT_FALSE(rightOperand.isNull());
	EXPECT_EQ("2", rightOperand->stringRepresentation());
}

TEST_F(TextLanguageParserTest, associativity)
{
	// Left-associative addition.
	QString stream = "1 + 2 + 3";
	auto result = mParser->parse(stream);
	EXPECT_TRUE(result.errors.isEmpty());

	auto firstAddition = result.astRoot.dynamicCast<ast::Addition>();
	ASSERT_FALSE(firstAddition.isNull());

	auto secondAddition = firstAddition->leftOperand().dynamicCast<ast::Addition>();
	ASSERT_FALSE(secondAddition.isNull());

	auto one = secondAddition->leftOperand().dynamicCast<ast::Number>();
	ASSERT_FALSE(one.isNull());
	EXPECT_EQ("1", one->stringRepresentation());

	auto two = secondAddition->rightOperand().dynamicCast<ast::Number>();
	ASSERT_FALSE(two.isNull());
	EXPECT_EQ("2", two->stringRepresentation());

	auto three = firstAddition->rightOperand().dynamicCast<ast::Number>();
	ASSERT_FALSE(three.isNull());
	EXPECT_EQ("3", three->stringRepresentation());

	// Right-associative exponentiation.
	stream = "1 ^ 2 ^ 3";
	result = mParser->parse(stream);
	EXPECT_TRUE(result.errors.isEmpty());

	auto firstExponentiation = result.astRoot.dynamicCast<ast::Exponentiation>();
	ASSERT_FALSE(firstExponentiation.isNull());

	one = firstExponentiation->leftOperand().dynamicCast<ast::Number>();
	ASSERT_FALSE(one.isNull());
	EXPECT_EQ("1", one->stringRepresentation());

	auto secondExponentiation = firstExponentiation->rightOperand().dynamicCast<ast::Exponentiation>();
	ASSERT_FALSE(secondExponentiation.isNull());

	two = secondExponentiation->leftOperand().dynamicCast<ast::Number>();
	ASSERT_FALSE(two.isNull());
	EXPECT_EQ("2", two->stringRepresentation());

	three = secondExponentiation->rightOperand().dynamicCast<ast::Number>();
	ASSERT_FALSE(three.isNull());
	EXPECT_EQ("3", three->stringRepresentation());
}

TEST_F(TextLanguageParserTest, precedence)
{
	QString const stream = "1 * 2 + 3 * 4 + 5 * 6";
	auto result = mParser->parse(stream);
	EXPECT_TRUE(result.errors.isEmpty());

	auto firstAddition = result.astRoot.dynamicCast<ast::Addition>();
	ASSERT_FALSE(firstAddition.isNull());

	auto secondAddition = firstAddition->leftOperand().dynamicCast<ast::Addition>();
	ASSERT_FALSE(secondAddition.isNull());

	auto firstMultiplication = secondAddition->leftOperand().dynamicCast<ast::Multiplication>();
	ASSERT_FALSE(firstMultiplication.isNull());

	auto one = firstMultiplication->leftOperand().dynamicCast<ast::Number>();
	ASSERT_FALSE(one.isNull());
	EXPECT_EQ("1", one->stringRepresentation());

	auto two = firstMultiplication->rightOperand().dynamicCast<ast::Number>();
	ASSERT_FALSE(two.isNull());
	EXPECT_EQ("2", two->stringRepresentation());

	auto secondMultiplication = secondAddition->rightOperand().dynamicCast<ast::Multiplication>();
	ASSERT_FALSE(secondMultiplication.isNull());

	auto three = secondMultiplication->leftOperand().dynamicCast<ast::Number>();
	ASSERT_FALSE(three.isNull());
	EXPECT_EQ("3", three->stringRepresentation());

	auto four = secondMultiplication->rightOperand().dynamicCast<ast::Number>();
	ASSERT_FALSE(four.isNull());
	EXPECT_EQ("4", four->stringRepresentation());

	auto thirdMultiplication = firstAddition->rightOperand().dynamicCast<ast::Multiplication>();
	ASSERT_FALSE(thirdMultiplication.isNull());

	auto five = thirdMultiplication->leftOperand().dynamicCast<ast::Number>();
	ASSERT_FALSE(five.isNull());
	EXPECT_EQ("5", five->stringRepresentation());

	auto six = thirdMultiplication->rightOperand().dynamicCast<ast::Number>();
	ASSERT_FALSE(six.isNull());
	EXPECT_EQ("6", six->stringRepresentation());
}

TEST_F(TextLanguageParserTest, tableConstructor)
{
	// Empty table constructor
	QString stream = "{}";
	auto result = mParser->parse(stream);
	EXPECT_TRUE(result.errors.isEmpty());

	QSharedPointer<ast::TableConstructor> emptyConstructor = result.astRoot.dynamicCast<ast::TableConstructor>();
	ASSERT_FALSE(emptyConstructor.isNull());
	EXPECT_TRUE(emptyConstructor->initializers().isEmpty());

	// Some simple initializers
	stream = "{ 1, [30] = 23; 45 }";
	result = mParser->parse(stream);
	EXPECT_TRUE(result.errors.isEmpty());
	QSharedPointer<ast::TableConstructor> constructor = result.astRoot.dynamicCast<ast::TableConstructor>();
	ASSERT_FALSE(constructor.isNull());
	EXPECT_EQ(3, constructor->initializers().size());

	// Test from Lua reference
	stream = "{ [f(1)] = g; \"x\", \"y\"; x = 1, f(x), [30] = 23; 45 }";
	result = mParser->parse(stream);
	EXPECT_TRUE(result.errors.isEmpty());
	constructor = result.astRoot.dynamicCast<ast::TableConstructor>();
	ASSERT_FALSE(constructor.isNull());
	EXPECT_EQ(7, constructor->initializers().size());
}

TEST_F(TextLanguageParserTest, identifier)
{
	QString const stream = "f";
	auto result = mParser->parse(stream);
	EXPECT_TRUE(result.errors.isEmpty());
	auto identifier = result.astRoot.dynamicCast<ast::Identifier>();
	ASSERT_FALSE(identifier.isNull());
	EXPECT_EQ("f", identifier->name());
}

TEST_F(TextLanguageParserTest, tableIndexing)
{
	// Indexers with square brackets
	QString stream = "t[1]";
	auto result = mParser->parse(stream);
	EXPECT_TRUE(result.errors.isEmpty());
	auto indexingExpression = as<ast::IndexingExpression>(result.astRoot);
	ASSERT_FALSE(indexingExpression.isNull());

	auto table = indexingExpression->table();
	ASSERT_TRUE(table->is<ast::Identifier>());
	EXPECT_EQ("t", as<ast::Identifier>(table)->name());

	auto indexer = indexingExpression->indexer();
	ASSERT_TRUE(indexer->is<ast::IntegerNumber>());
	EXPECT_EQ("1", as<ast::IntegerNumber>(indexer)->stringRepresentation());

	// "Field-like" indexers
	stream = "t.x";
	result = mParser->parse(stream);
	EXPECT_TRUE(result.errors.isEmpty());
	indexingExpression = as<ast::IndexingExpression>(result.astRoot);
	ASSERT_FALSE(indexingExpression.isNull());

	table = indexingExpression->table();
	ASSERT_TRUE(table->is<ast::Identifier>());
	EXPECT_EQ("t", as<ast::Identifier>(table)->name());

	indexer = indexingExpression->indexer();
	ASSERT_TRUE(indexer->is<ast::String>());
	EXPECT_EQ("x", as<ast::String>(indexer)->string());
}

TEST_F(TextLanguageParserTest, functionCalls)
{
	// Functions
	QString stream = "f(x)";
	auto result = mParser->parse(stream);
	EXPECT_TRUE(result.errors.isEmpty());
	auto call = as<ast::FunctionCall>(result.astRoot);
	ASSERT_FALSE(call.isNull());

	auto function = call->function();
	ASSERT_TRUE(function->is<ast::Identifier>());
	EXPECT_EQ("f", as<ast::Identifier>(function)->name());

	ASSERT_EQ(1, call->arguments().size());
	auto argument = call->arguments().first();
	ASSERT_TRUE(argument->is<ast::Identifier>());
	EXPECT_EQ("x", as<ast::Identifier>(argument)->name());

	// Methods
	stream = "a:m(x)";
	result = mParser->parse(stream);
	EXPECT_TRUE(result.errors.isEmpty());
	auto methodCall = as<ast::MethodCall>(result.astRoot);
	ASSERT_FALSE(call.isNull());

	auto object = methodCall->object();
	ASSERT_TRUE(object->is<ast::Identifier>());
	EXPECT_EQ("a", as<ast::Identifier>(object)->name());

	function = methodCall->methodName();
	ASSERT_TRUE(function->is<ast::Identifier>());
	EXPECT_EQ("m", as<ast::Identifier>(function)->name());

	ASSERT_EQ(1, methodCall->arguments().size());
	argument = methodCall->arguments().first();
	ASSERT_TRUE(argument->is<ast::Identifier>());
	EXPECT_EQ("x", as<ast::Identifier>(argument)->name());

	// More complex case, calling method of some unknown object
	stream = "(a[1].someObject):method(1, 2, 3)";
	result = mParser->parse(stream);
	EXPECT_TRUE(result.errors.isEmpty());
	methodCall = as<ast::MethodCall>(result.astRoot);
	ASSERT_FALSE(call.isNull());

	object = methodCall->object();
	ASSERT_TRUE(object->is<ast::IndexingExpression>());
	ASSERT_TRUE(as<ast::IndexingExpression>(object)->indexer()->is<ast::String>());
	EXPECT_EQ("someObject", as<ast::String>(as<ast::IndexingExpression>(object)->indexer())->string());

	function = methodCall->methodName();
	ASSERT_TRUE(function->is<ast::Identifier>());
	EXPECT_EQ("method", as<ast::Identifier>(function)->name());

	ASSERT_EQ(3, methodCall->arguments().size());
	argument = methodCall->arguments().first();
	ASSERT_TRUE(argument->is<ast::IntegerNumber>());
	EXPECT_EQ("1", as<ast::IntegerNumber>(argument)->stringRepresentation());

	argument = methodCall->arguments()[1];
	ASSERT_TRUE(argument->is<ast::IntegerNumber>());
	EXPECT_EQ("2", as<ast::IntegerNumber>(argument)->stringRepresentation());

	argument = methodCall->arguments()[2];
	ASSERT_TRUE(argument->is<ast::IntegerNumber>());
	EXPECT_EQ("3", as<ast::IntegerNumber>(argument)->stringRepresentation());
}
