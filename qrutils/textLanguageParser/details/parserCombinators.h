#pragma once

#include "textLanguageParser/details/parserInterface.h"
#include "textLanguageParser/details/simpleParser.h"
#include "textLanguageParser/details/alternativeParser.h"
#include "textLanguageParser/details/transformingParser.h"
#include "textLanguageParser/details/concatenationParser.h"
#include "textLanguageParser/tokenType.h"

namespace textLanguageParser {
namespace details {

template<typename P1, typename P2>
inline ConcatenationParser<P1, P2> operator + (P1 a, P2 b) {
	return ConcatenationParser<P1, P2>(a, b);
}

template<typename P1, typename P2>
inline AlternativeParser<P1, P2> operator | (P1 a, P2 b) {
	return AlternativeParser<P1, P2>(a, b);
}

inline SimpleParser operator -(TokenType tokenType) {
	return SimpleParser(tokenType);
}

template<typename Transformation, typename P>
inline TransformingParser<Transformation, P> operator >>(P parser, Transformation transformation)
{
	return TransformingParser<Transformation, P>(parser, transformation);
}

}
}
