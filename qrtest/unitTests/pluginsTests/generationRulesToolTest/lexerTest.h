#pragma once

#include <QtCore/QScopedPointer>
#include <QtCore/QString>

#include <plugins/generationRulesTool/lexer.h>
#include <gtest/gtest.h>

namespace qrTest {

class LexerTest : public testing::Test
{
protected:
	virtual void SetUp();

	virtual void TearDown();

	QScopedPointer<simpleParser::Lexer> mLexer;
	QList<qrtext::core::Error> mErrors;
};

}
