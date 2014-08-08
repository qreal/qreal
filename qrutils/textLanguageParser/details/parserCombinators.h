#pragma once

#include <type_traits>

#include <QtCore/QSharedPointer>

#include "textLanguageParser/details/parserRef.h"
#include "textLanguageParser/details/parserInterface.h"
#include "textLanguageParser/details/simpleParser.h"
#include "textLanguageParser/details/tokenParser.h"
#include "textLanguageParser/details/alternativeParser.h"
#include "textLanguageParser/details/transformingParser.h"
#include "textLanguageParser/details/concatenationParser.h"
#include "textLanguageParser/details/optionalParser.h"
#include "textLanguageParser/details/kleeneStarParser.h"
#include "textLanguageParser/details/functionTraits.h"
#include "textLanguageParser/tokenType.h"

namespace textLanguageParser {
namespace details {

inline ParserRef operator & (ParserRef const &a, ParserRef const &b) {
	return ParserRef(new ConcatenationParser(a, b));
}

inline ParserRef operator | (ParserRef const &a, ParserRef const &b) {
	return ParserRef(new AlternativeParser(a, b));
}

inline ParserRef operator ~ (ParserRef a) {
	return ParserRef(new OptionalParser(a));
}

inline ParserRef operator * (ParserRef a) {
	return ParserRef(new KleeneStarParser(a));
}

template<typename SemanticAction>
inline ParserRef operator >>(TokenType token, SemanticAction semanticAction)
{
	typedef typename std::conditional<function_traits<SemanticAction>::arity == 0
			, SimpleParser<SemanticAction>
			, TokenParser<SemanticAction>>
			::type Parser;

	return ParserRef(new Parser(token, semanticAction));
}

template<typename Transformation>
inline ParserRef operator >>(ParserRef parser, Transformation transformation)
{
	return ParserRef(new TransformingParser<Transformation>(parser, transformation));
}

inline ParserRef operator & (TokenType const &token, ParserRef const &b) {
	return ParserRef(new ConcatenationParser(token >> [] { return new ast::Node(); }, b));
}

inline ParserRef operator & (ParserRef const &a, TokenType const &token) {
	return ParserRef(new ConcatenationParser(a, token >> [] { return new ast::Node(); }));
}

inline ParserRef operator & (TokenType const &token1, TokenType const &token2) {
	return ParserRef(new ConcatenationParser(
			token1 >> [] { return new ast::Node(); },
			token2 >> [] { return new ast::Node(); })
			);
}

}
}
