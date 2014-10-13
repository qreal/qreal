#pragma once

#include <QtCore/QScopedPointer>

#include <gtest/gtest.h>

#include "textLanguageParser/textLanguageFacade.h"

namespace qrTest {

class TextLanguageFacadeTest : public testing::Test
{
protected:
	void SetUp() override;

	QSharedPointer<textLanguageParser::ast::Node> parse(QString const &code);

	QScopedPointer<textLanguageParser::TextLanguageFacade> mFacade;
};

}
