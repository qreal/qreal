#include "semanticAnalyzerTest.h"

#include "textLanguageParser/textLanguageParser.h"

#include "gtest/gtest.h"

using namespace textLanguageParser;
using namespace qrTest;

void SemanticAnalyzerTest::SetUp()
{
	mAnalyzer.reset(new SemanticAnalyzer());
}

QSharedPointer<textLanguageParser::ast::Node> SemanticAnalyzerTest::parse(QString const &code)
{
	TextLanguageParser parser;
	return parser.parse(code).astRoot;
}

TEST_F(SemanticAnalyzerTest, sanityCheck)
{
	QList<ParserError> errors;
	auto tree = parse("123");
	mAnalyzer->analyze(tree, errors);
}
