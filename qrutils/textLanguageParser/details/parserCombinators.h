#pragma once

#include <QtCore/QSharedPointer>

#include "textLanguageParser/details/parserRef.h"
#include "textLanguageParser/details/parserInterface.h"
#include "textLanguageParser/details/simpleParser.h"
#include "textLanguageParser/details/alternativeParser.h"
#include "textLanguageParser/details/transformingParser.h"
#include "textLanguageParser/details/concatenationParser.h"
#include "textLanguageParser/tokenType.h"

namespace textLanguageParser {
namespace details {

inline ParserRef operator + (ParserRef a, ParserRef b) {
	return ParserRef(new ConcatenationParser(a, b));
}

inline ParserRef operator | (ParserRef a, ParserRef b) {
	return ParserRef(new AlternativeParser(a, b));
}

inline ParserRef operator -(TokenType tokenType) {
	return ParserRef(new SimpleParser(tokenType));
}

template<typename Transformation>
inline ParserRef operator >>(ParserRef parser, Transformation transformation)
{
	return ParserRef(new TransformingParser<Transformation>(parser, transformation));
}

}
}
