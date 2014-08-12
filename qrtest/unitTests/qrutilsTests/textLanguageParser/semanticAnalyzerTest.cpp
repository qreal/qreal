#include "semanticAnalyzerTest.h"

#include "textLanguageParser/textLanguageParser.h"
#include "textLanguageParser/ast/nodes/assignment.h"

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
	ASSERT_TRUE(mAnalyzer->type(tree)->is<types::Integer>());
	ASSERT_TRUE(mAnalyzer->type(tree)->is<types::Number>());
}

TEST_F(SemanticAnalyzerTest, assignment)
{
	auto tree = parse("a = 123");

	auto variable = as<ast::Assignment>(tree)->variable();
	auto value = as<ast::Assignment>(tree)->value();

	mAnalyzer->analyze(tree);
	ASSERT_TRUE(mAnalyzer->type(variable)->is<types::Integer>());
	ASSERT_TRUE(mAnalyzer->type(value)->is<types::Integer>());
}
