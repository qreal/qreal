#pragma once

#include <QtCore/QScopedPointer>
#include <QtCore/QString>

#include <plugins/generationRulesTool/parser.h>
#include <gtest/gtest.h>

namespace qrTest {

class ParserTest: public testing::Test
{
protected:
	virtual void SetUp();

	virtual void TearDown();

	QScopedPointer<simpleParser::Parser> mParser;
	QList<qrtext::core::Error> mErrors;
};

}
