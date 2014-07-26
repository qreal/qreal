#pragma once

#include <QtCore/QList>
#include <QtCore/QString>

#include "textLanguageParser/details/token.h"
#include "textLanguageParser/parserError.h"
#include "textLanguageParser/lexemes.h"

namespace textLanguageParser {
namespace details {

class Lexer {
public:
	struct Result {
		QList<Token> tokens;
		QList<ParserError> errors;
		QList<Token> comments;
	};

	explicit Lexer(Lexemes const &lexemes);

	Result tokenize(QString const &input);

private:
	Lexemes const &mLexemes;
};

}
}
