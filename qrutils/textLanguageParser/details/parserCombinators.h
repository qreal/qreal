#pragma once

#include <QtCore/QSharedPointer>

#include "textLanguageParser/details/parserRef.h"
#include "textLanguageParser/details/parserInterface.h"
#include "textLanguageParser/details/simpleParser.h"
#include "textLanguageParser/details/alternativeParser.h"
#include "textLanguageParser/details/transformingParser.h"
#include "textLanguageParser/details/concatenationParser.h"
#include "textLanguageParser/details/optionalParser.h"
#include "textLanguageParser/details/kleeneStarParser.h"
#include "textLanguageParser/tokenType.h"

namespace textLanguageParser {
namespace details {

inline ParserRef operator + (ParserRef a, ParserRef b) {
	return ParserRef(new ConcatenationParser(a, b));
}

inline ParserRef operator | (ParserRef a, ParserRef b) {
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
	return ParserRef(new SimpleParser<SemanticAction>(token, semanticAction));
}

template<typename Transformation>
inline ParserRef operator >>(ParserRef parser, Transformation transformation)
{
	return ParserRef(new TransformingParser<Transformation>(parser, transformation));
}

}
}
