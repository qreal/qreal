#include "luaSemanticAnalyzerTest.h"

#include "qrtext/lua/ast/assignment.h"
#include "qrtext/lua/ast/block.h"
#include "qrtext/lua/ast/unaryMinus.h"

#include "qrtext/lua/types/integer.h"
#include "qrtext/lua/types/float.h"

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

QSharedPointer<qrtext::core::ast::Node> LuaSemanticAnalyzerTest::parse(QString const &code)
{
	return mParser->parse(mLexer->tokenize(code));
}

TEST_F(LuaSemanticAnalyzerTest, sanityCheck)
{
	auto tree = parse("123");
	mAnalyzer->analyze(tree);
	EXPECT_TRUE(mAnalyzer->type(tree)->is<Integer>());
	EXPECT_TRUE(mAnalyzer->type(tree)->is<Number>());
}

TEST_F(LuaSemanticAnalyzerTest, assignment)
{
	auto tree = parse("a = 123");

	auto variable = as<ast::Assignment>(tree)->variable();
	auto value = as<ast::Assignment>(tree)->value();

	mAnalyzer->analyze(tree);
	EXPECT_TRUE(mAnalyzer->type(variable)->is<types::Integer>());
	EXPECT_TRUE(mAnalyzer->type(value)->is<types::Integer>());
}

TEST_F(LuaSemanticAnalyzerTest, unaryOperator)
{
	auto tree = parse("-123");

	mAnalyzer->analyze(tree);
	EXPECT_TRUE(mAnalyzer->type(as<ast::UnaryMinus>(tree))->is<types::Integer>());
}

TEST_F(LuaSemanticAnalyzerTest, propagation)
{
	auto tree = parse("a = -123; b = a");

	mAnalyzer->analyze(tree);
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
