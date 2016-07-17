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

#include "luaParserTest.h"

#include "qrtext/lua/ast/unaryOperator.h"
#include "qrtext/lua/ast/binaryOperator.h"

#include "qrtext/lua/ast/number.h"
#include "qrtext/lua/ast/integerNumber.h"
#include "qrtext/lua/ast/floatNumber.h"

#include "qrtext/lua/ast/functionCall.h"
#include "qrtext/lua/ast/methodCall.h"
#include "qrtext/lua/ast/indexingExpression.h"
#include "qrtext/lua/ast/block.h"
#include "qrtext/lua/ast/assignment.h"
#include "qrtext/lua/ast/addition.h"
#include "qrtext/lua/ast/multiplication.h"
#include "qrtext/lua/ast/exponentiation.h"
#include "qrtext/lua/ast/tableConstructor.h"
#include "qrtext/lua/ast/identifier.h"
#include "qrtext/lua/ast/string.h"
#include "qrtext/lua/ast/true.h"
#include "qrtext/lua/ast/false.h"

#include "gtest/gtest.h"

using namespace qrtext;
using namespace qrtext::lua;
using namespace qrtext::lua::details;
using namespace qrTest;

void LuaParserTest::SetUp()
{
	mErrors.clear();
	mLexer.reset(new LuaLexer(mErrors));
	mParser.reset(new LuaParser(mErrors));
}

TEST_F(LuaParserTest, sanityCheck)
{
	const QString stream = "123";
	auto result = mParser->parse(mLexer->tokenize(stream), mLexer->userFriendlyTokenNames());
	EXPECT_TRUE(mErrors.isEmpty());
	auto number = result.dynamicCast<ast::Number>();
	ASSERT_FALSE(number.isNull());
	EXPECT_EQ("123", number->stringRepresentation());
}

TEST_F(LuaParserTest, unaryOp)
{
	const QString stream = "-123";
	auto result = mParser->parse(mLexer->tokenize(stream), mLexer->userFriendlyTokenNames());
	EXPECT_TRUE(mErrors.isEmpty());
	QSharedPointer<ast::UnaryOperator> unaryOp = result.dynamicCast<ast::UnaryOperator>();
	ASSERT_FALSE(unaryOp.isNull());
	QSharedPointer<ast::Number> operand = unaryOp->operand().dynamicCast<ast::Number>();
	ASSERT_FALSE(operand.isNull());
	EXPECT_EQ("123", operand->stringRepresentation());
}

TEST_F(LuaParserTest, connections)
{
	const QString stream = "-123";
	auto result = mParser->parse(mLexer->tokenize(stream), mLexer->userFriendlyTokenNames());
	EXPECT_TRUE(mErrors.isEmpty());
	QSharedPointer<ast::UnaryOperator> unaryOp = result.dynamicCast<ast::UnaryOperator>();
	ASSERT_FALSE(unaryOp.isNull());
	EXPECT_EQ(core::Connection(0, 0, 0), unaryOp->start());
	QSharedPointer<ast::Number> operand = unaryOp->operand().dynamicCast<ast::Number>();
	ASSERT_FALSE(operand.isNull());
	EXPECT_EQ(core::Connection(1, 0, 1), operand->start());
	EXPECT_EQ(core::Connection(3, 0, 3), operand->end());
}

TEST_F(LuaParserTest, binaryOp)
{
	const QString stream = "1+2";
	auto result = mParser->parse(mLexer->tokenize(stream), mLexer->userFriendlyTokenNames());
	EXPECT_TRUE(mErrors.isEmpty());
	QSharedPointer<ast::BinaryOperator> binaryOp = result.dynamicCast<ast::BinaryOperator>();
	ASSERT_FALSE(binaryOp.isNull());
	QSharedPointer<ast::Number> leftOperand = binaryOp->leftOperand().dynamicCast<ast::Number>();
	ASSERT_FALSE(leftOperand.isNull());
	EXPECT_EQ("1", leftOperand->stringRepresentation());
	QSharedPointer<ast::Number> rightOperand = binaryOp->rightOperand().dynamicCast<ast::Number>();
	ASSERT_FALSE(rightOperand.isNull());
	EXPECT_EQ("2", rightOperand->stringRepresentation());
}

TEST_F(LuaParserTest, associativity)
{
	// Left-associative addition.
	QString stream = "1 + 2 + 3";
	auto result = mParser->parse(mLexer->tokenize(stream), mLexer->userFriendlyTokenNames());
	EXPECT_TRUE(mErrors.isEmpty());

	auto firstAddition = result.dynamicCast<ast::Addition>();
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
	result = mParser->parse(mLexer->tokenize(stream), mLexer->userFriendlyTokenNames());
	EXPECT_TRUE(mErrors.isEmpty());

	auto firstExponentiation = result.dynamicCast<ast::Exponentiation>();
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

TEST_F(LuaParserTest, precedence)
{
	const QString stream = "1 * 2 + 3 * 4 + 5 * 6";
	auto result = mParser->parse(mLexer->tokenize(stream), mLexer->userFriendlyTokenNames());
	EXPECT_TRUE(mErrors.isEmpty());

	auto firstAddition = result.dynamicCast<ast::Addition>();
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

TEST_F(LuaParserTest, tableConstructor)
{
	// Empty table constructor
	QString stream = "{}";
	auto result = mParser->parse(mLexer->tokenize(stream), mLexer->userFriendlyTokenNames());
	EXPECT_TRUE(mErrors.isEmpty());

	QSharedPointer<ast::TableConstructor> emptyConstructor = result.dynamicCast<ast::TableConstructor>();
	ASSERT_FALSE(emptyConstructor.isNull());
	EXPECT_TRUE(emptyConstructor->initializers().isEmpty());

	// Some simple initializers
	stream = "{ 1, [30] = 23; 45 }";
	result = mParser->parse(mLexer->tokenize(stream), mLexer->userFriendlyTokenNames());
	EXPECT_TRUE(mErrors.isEmpty());
	QSharedPointer<ast::TableConstructor> constructor = result.dynamicCast<ast::TableConstructor>();
	ASSERT_FALSE(constructor.isNull());
	EXPECT_EQ(3, constructor->initializers().size());

	// Test from Lua reference
	stream = "{ [f(1)] = g; \"x\", \"y\"; x = 1, f(x), [30] = 23; 45 }";
	result = mParser->parse(mLexer->tokenize(stream), mLexer->userFriendlyTokenNames());
	EXPECT_TRUE(mErrors.isEmpty());
	constructor = result.dynamicCast<ast::TableConstructor>();
	ASSERT_FALSE(constructor.isNull());
	EXPECT_EQ(7, constructor->initializers().size());
}

TEST_F(LuaParserTest, identifier)
{
	const QString stream = "f";
	auto result = mParser->parse(mLexer->tokenize(stream), mLexer->userFriendlyTokenNames());
	EXPECT_TRUE(mErrors.isEmpty());
	auto identifier = result.dynamicCast<ast::Identifier>();
	ASSERT_FALSE(identifier.isNull());
	EXPECT_EQ("f", identifier->name());
}

TEST_F(LuaParserTest, tableIndexing)
{
	// Indexers with square brackets
	QString stream = "t[1]";
	auto result = mParser->parse(mLexer->tokenize(stream), mLexer->userFriendlyTokenNames());
	EXPECT_TRUE(mErrors.isEmpty());
	auto indexingExpression = as<ast::IndexingExpression>(result);
	ASSERT_FALSE(indexingExpression.isNull());

	auto table = indexingExpression->table();
	ASSERT_TRUE(table->is<ast::Identifier>());
	EXPECT_EQ("t", as<ast::Identifier>(table)->name());

	auto indexer = indexingExpression->indexer();
	ASSERT_TRUE(indexer->is<ast::IntegerNumber>());
	EXPECT_EQ("1", as<ast::IntegerNumber>(indexer)->stringRepresentation());

	// "Field-like" indexers
	stream = "t.x";
	result = mParser->parse(mLexer->tokenize(stream), mLexer->userFriendlyTokenNames());
	EXPECT_TRUE(mErrors.isEmpty());
	indexingExpression = as<ast::IndexingExpression>(result);
	ASSERT_FALSE(indexingExpression.isNull());

	table = indexingExpression->table();
	ASSERT_TRUE(table->is<ast::Identifier>());
	EXPECT_EQ("t", as<ast::Identifier>(table)->name());

	indexer = indexingExpression->indexer();
	ASSERT_TRUE(indexer->is<ast::String>());
	EXPECT_EQ("x", as<ast::String>(indexer)->string());
}

TEST_F(LuaParserTest, functionCalls)
{
	// Functions
	QString stream = "f(x)";
	auto result = mParser->parse(mLexer->tokenize(stream), mLexer->userFriendlyTokenNames());
	EXPECT_TRUE(mErrors.isEmpty());
	auto call = as<ast::FunctionCall>(result);
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
	result = mParser->parse(mLexer->tokenize(stream), mLexer->userFriendlyTokenNames());
	EXPECT_TRUE(mErrors.isEmpty());
	auto methodCall = as<ast::MethodCall>(result);
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
	result = mParser->parse(mLexer->tokenize(stream), mLexer->userFriendlyTokenNames());
	EXPECT_TRUE(mErrors.isEmpty());
	methodCall = as<ast::MethodCall>(result);
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

TEST_F(LuaParserTest, tableIndexingConnection)
{
	//                0    5    10   1517
	QString stream = "a[1] = 1; a[2] = 2";
	auto result = mParser->parse(mLexer->tokenize(stream), mLexer->userFriendlyTokenNames());
	EXPECT_TRUE(mErrors.isEmpty());

	auto block = as<ast::Block>(result);

	auto firstAssignment = as<ast::Assignment>(block->children()[0]);
	auto secondAssignment = as<ast::Assignment>(block->children()[1]);

	auto firstIndexingExpression = as<ast::IndexingExpression>(firstAssignment->variable());
	auto secondIndexingExpression = as<ast::IndexingExpression>(secondAssignment->variable());

	auto firstAssignee = as<ast::IntegerNumber>(firstAssignment->value());
	auto secondAssignee = as<ast::IntegerNumber>(secondAssignment->value());

	auto firstIndexer = as<ast::IntegerNumber>(firstIndexingExpression->indexer());
	auto secondIndexer = as<ast::IntegerNumber>(secondIndexingExpression->indexer());

	auto firstTable = as<ast::Identifier>(firstIndexingExpression->table());
	auto secondTable = as<ast::Identifier>(secondIndexingExpression->table());

	EXPECT_EQ(core::Connection(0, 0, 0), firstAssignment->start());
	EXPECT_EQ(core::Connection(10, 0, 10), secondAssignment->start());

	EXPECT_EQ(core::Connection(0, 0, 0), firstIndexingExpression->start());
	EXPECT_EQ(core::Connection(10, 0, 10), secondIndexingExpression->start());

	EXPECT_EQ(core::Connection(7, 0, 7), firstAssignee->start());
	EXPECT_EQ(core::Connection(17, 0, 17), secondAssignee->start());

	EXPECT_EQ(core::Connection(2, 0, 2), firstIndexer->start());
	EXPECT_EQ(core::Connection(12, 0, 12), secondIndexer->start());

	EXPECT_EQ(core::Connection(0, 0, 0), firstTable->start());
	EXPECT_EQ(core::Connection(10, 0, 10), secondTable->start());
}

TEST_F(LuaParserTest, logicalOperators)
{
	QString stream = "true or false";
	auto result = mParser->parse(mLexer->tokenize(stream), mLexer->userFriendlyTokenNames());
	EXPECT_TRUE(mErrors.isEmpty());

	QSharedPointer<ast::BinaryOperator> binaryOp = result.dynamicCast<ast::BinaryOperator>();
	ASSERT_FALSE(binaryOp.isNull());
	auto leftOperand = binaryOp->leftOperand().dynamicCast<ast::True>();
	ASSERT_FALSE(leftOperand.isNull());
	auto rightOperand = binaryOp->rightOperand().dynamicCast<ast::False>();
	ASSERT_FALSE(rightOperand.isNull());

	stream = "true and false";
	result = mParser->parse(mLexer->tokenize(stream), mLexer->userFriendlyTokenNames());
	EXPECT_TRUE(mErrors.isEmpty());

	binaryOp = result.dynamicCast<ast::BinaryOperator>();
	ASSERT_FALSE(binaryOp.isNull());
	leftOperand = binaryOp->leftOperand().dynamicCast<ast::True>();
	ASSERT_FALSE(leftOperand.isNull());
	rightOperand = binaryOp->rightOperand().dynamicCast<ast::False>();
	ASSERT_FALSE(rightOperand.isNull());

	stream = "true && false";
	result = mParser->parse(mLexer->tokenize(stream), mLexer->userFriendlyTokenNames());
	EXPECT_TRUE(mErrors.isEmpty());

	binaryOp = result.dynamicCast<ast::BinaryOperator>();
	ASSERT_FALSE(binaryOp.isNull());
	leftOperand = binaryOp->leftOperand().dynamicCast<ast::True>();
	ASSERT_FALSE(leftOperand.isNull());
	rightOperand = binaryOp->rightOperand().dynamicCast<ast::False>();
	ASSERT_FALSE(rightOperand.isNull());

	stream = "true || false";
	result = mParser->parse(mLexer->tokenize(stream), mLexer->userFriendlyTokenNames());
	EXPECT_TRUE(mErrors.isEmpty());

	binaryOp = result.dynamicCast<ast::BinaryOperator>();
	ASSERT_FALSE(binaryOp.isNull());
	leftOperand = binaryOp->leftOperand().dynamicCast<ast::True>();
	ASSERT_FALSE(leftOperand.isNull());
	rightOperand = binaryOp->rightOperand().dynamicCast<ast::False>();
	ASSERT_FALSE(rightOperand.isNull());

	stream = "not false";
	result = mParser->parse(mLexer->tokenize(stream), mLexer->userFriendlyTokenNames());
	EXPECT_TRUE(mErrors.isEmpty());

	QSharedPointer<ast::UnaryOperator>  unaryOp = result.dynamicCast<ast::UnaryOperator>();
	ASSERT_FALSE(unaryOp.isNull());
	rightOperand = unaryOp->operand().dynamicCast<ast::False>();
	ASSERT_FALSE(leftOperand.isNull());
}

TEST_F(LuaParserTest, concatenation)
{
	const QString stream = "'1' .. '2'";
	auto result = mParser->parse(mLexer->tokenize(stream), mLexer->userFriendlyTokenNames());
	EXPECT_TRUE(mErrors.isEmpty());
	QSharedPointer<ast::BinaryOperator> binaryOp = result.dynamicCast<ast::BinaryOperator>();
	ASSERT_FALSE(binaryOp.isNull());
	QSharedPointer<ast::String> leftOperand = binaryOp->leftOperand().dynamicCast<ast::String>();
	ASSERT_FALSE(leftOperand.isNull());
	EXPECT_EQ("1", leftOperand->string());
	QSharedPointer<ast::String> rightOperand = binaryOp->rightOperand().dynamicCast<ast::String>();
	ASSERT_FALSE(rightOperand.isNull());
	EXPECT_EQ("2", rightOperand->string());
}

TEST_F(LuaParserTest, twoDTableInitializer)
{
	const QString stream = "{{1}, {2, 3}}";
	const auto result = mParser->parse(mLexer->tokenize(stream), mLexer->userFriendlyTokenNames());
	EXPECT_TRUE(mErrors.isEmpty());
	const QSharedPointer<ast::TableConstructor> outerTable = result.dynamicCast<ast::TableConstructor>();
	ASSERT_FALSE(outerTable.isNull());

	ASSERT_EQ(2, outerTable->initializers().size());

	const QSharedPointer<ast::FieldInitialization> outerFieldInitializer
			= outerTable->initializers()[0].dynamicCast<ast::FieldInitialization>();

	const QSharedPointer<ast::TableConstructor> innerTable
			= outerFieldInitializer->value().dynamicCast<ast::TableConstructor>();

	ASSERT_FALSE(innerTable.isNull());

	ASSERT_EQ(1, innerTable->initializers().size());

	const QSharedPointer<ast::FieldInitialization> innerFieldInitializer
			= innerTable->initializers()[0].dynamicCast<ast::FieldInitialization>();

	const QSharedPointer<ast::IntegerNumber> value
			= innerFieldInitializer->value().dynamicCast<ast::IntegerNumber>();

	ASSERT_FALSE(value.isNull());
	EXPECT_EQ(QString("1"), value->stringRepresentation());
}

TEST_F(LuaParserTest, twoDTableIndexer)
{
	const QString stream = "a[1][2]";
	const auto result = mParser->parse(mLexer->tokenize(stream), mLexer->userFriendlyTokenNames());
	EXPECT_TRUE(mErrors.isEmpty());

	const QSharedPointer<ast::IndexingExpression> outerIndexingExpression
			= result.dynamicCast<ast::IndexingExpression>();

	ASSERT_FALSE(outerIndexingExpression.isNull());

	const QSharedPointer<ast::IntegerNumber> outerIndexer
			= outerIndexingExpression->indexer().dynamicCast<ast::IntegerNumber>();
	ASSERT_EQ(QString("2"), outerIndexer->stringRepresentation());

	const QSharedPointer<ast::IndexingExpression> innerIndexingExpression
			= outerIndexingExpression->table().dynamicCast<ast::IndexingExpression>();

	ASSERT_FALSE(innerIndexingExpression.isNull());

	const QSharedPointer<ast::IntegerNumber> innerIndexer
			= innerIndexingExpression->indexer().dynamicCast<ast::IntegerNumber>();
	ASSERT_EQ(QString("1"), innerIndexer->stringRepresentation());
}
