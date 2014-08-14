#include "semanticAnalyzerTest.h"

#include "textLanguageParser/ast/nodes/block.h"

#include "textLanguageParser/textLanguageParser.h"
#include "textLanguageParser/ast/nodes/assignment.h"
#include "textLanguageParser/ast/nodes/unaryMinus.h"

#include "gtest/gtest.h"

using namespace textLanguageParser;
using namespace qrTest;

void SemanticAnalyzerTest::SetUp()
{
	mAnalyzer.reset(new SemanticAnalyzer(mErrors));
}

QSharedPointer<textLanguageParser::ast::Node> SemanticAnalyzerTest::parse(QString const &code)
{
	TextLanguageParser parser;
	return parser.parse(code).astRoot;
}

TEST_F(SemanticAnalyzerTest, sanityCheck)
{
	auto tree = parse("123");
	mAnalyzer->analyze(tree);
	EXPECT_TRUE(mAnalyzer->type(tree)->is<types::Integer>());
	EXPECT_TRUE(mAnalyzer->type(tree)->is<types::Number>());
}

TEST_F(SemanticAnalyzerTest, assignment)
{
	auto tree = parse("a = 123");

	auto variable = as<ast::Assignment>(tree)->variable();
	auto value = as<ast::Assignment>(tree)->value();

	mAnalyzer->analyze(tree);
	EXPECT_TRUE(mAnalyzer->type(variable)->is<types::Integer>());
	EXPECT_TRUE(mAnalyzer->type(value)->is<types::Integer>());
}

TEST_F(SemanticAnalyzerTest, unaryOperator)
{
	auto tree = parse("-123");

	mAnalyzer->analyze(tree);
	EXPECT_TRUE(mAnalyzer->type(as<ast::UnaryMinus>(tree))->is<types::Integer>());
}

TEST_F(SemanticAnalyzerTest, propagation)
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

TEST_F(SemanticAnalyzerTest, integerFloatCoercion)
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
