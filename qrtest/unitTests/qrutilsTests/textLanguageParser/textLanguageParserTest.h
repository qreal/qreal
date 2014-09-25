#pragma once

#include <QtCore/QScopedPointer>

#include <gtest/gtest.h>

#include "textLanguageParser/textLanguageParser.h"

namespace qrTest {

class TextLanguageParserTest : public testing::Test
{
protected:
	void SetUp() override;

	QScopedPointer<textLanguageParser::TextLanguageParserInterface> mParser;
};

}
