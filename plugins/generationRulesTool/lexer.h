#pragma once

#include <QtCore/QScopedPointer>

#include <qrtext/core/lexer/lexer.h>
#include "tokenTypes.h"

namespace simpleParser {

/// Lexer for generation rules tool language.
class Lexer: public qrtext::core::Lexer<TokenTypes>
{
public:
	/// Constructor.
	/// @param errors - error stream to report errors to.
	Lexer(QList<qrtext::core::Error> &errors);

private:
	static qrtext::core::TokenPatterns<TokenTypes> initPatterns();
};
}
