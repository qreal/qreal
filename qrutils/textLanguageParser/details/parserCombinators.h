#pragma once

#include "textLanguageParser/details/parserInterface.h"
#include "textLanguageParser/details/simpleParser.h"
#include "textLanguageParser/details/alternativeParser.h"
#include "textLanguageParser/details/transformingParser.h"
#include "textLanguageParser/tokenType.h"

namespace textLanguageParser {
namespace details {

//inline ParserInterface &operator + (ParserInterface &a, ParserInterface &b) {
//	return new
//}

inline AlternativeParser operator | (ParserInterface const &a, ParserInterface const &b) {
	return AlternativeParser(a, b);
}

inline SimpleParser operator -(TokenType tokenType) {
	return SimpleParser(tokenType);
}

template<typename Transformation>
inline TransformingParser<Transformation> operator >>(ParserInterface const &parser
		, Transformation transformation)
{
	return TransformingParser<Transformation>(parser, transformation);
}

}
}
