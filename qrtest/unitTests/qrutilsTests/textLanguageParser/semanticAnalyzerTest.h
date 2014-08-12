#pragma once

#include <QtCore/QScopedPointer>

#include <gtest/gtest.h>

#include "textLanguageParser/semanticAnalyzer.h"
#include "textLanguageParser/parserError.h"
#include "textLanguageParser/ast/nodes/node.h"

namespace qrTest {

class SemanticAnalyzerTest : public testing::Test
{
protected:
	void SetUp() override;

	QSharedPointer<textLanguageParser::ast::Node> parse(QString const &code);

	QScopedPointer<textLanguageParser::SemanticAnalyzer> mAnalyzer;
	QList<textLanguageParser::ParserError> mErrors;
};

}
