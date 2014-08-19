#pragma once

#include <type_traits>

#include <QtCore/QSharedPointer>

#include "qrtext/core/parser/parserRef.h"
#include "qrtext/core/parser/operators/parserInterface.h"
#include "qrtext/core/parser/operators/simpleParser.h"
#include "qrtext/core/parser/operators/tokenParser.h"
#include "qrtext/core/parser/operators/alternativeParser.h"
#include "qrtext/core/parser/operators/transformingParser.h"
#include "qrtext/core/parser/operators/concatenationParser.h"
#include "qrtext/core/parser/operators/optionalParser.h"
#include "qrtext/core/parser/operators/kleeneStarParser.h"
#include "qrtext/core/parser/temporaryNodes/temporaryDiscardableNode.h"
#include "qrtext/core/parser/utils/functionTraits.h"

namespace qrtext {
namespace core {
namespace parser {

template<typename TokenType>
inline ParserRef<TokenType> operator & (ParserRef<TokenType> const &a, ParserRef<TokenType> const &b) {
	return ParserRef<TokenType>(new ConcatenationParser<TokenType>(a, b));
}

template<typename TokenType>
inline ParserRef<TokenType> operator | (ParserRef<TokenType> const &a, ParserRef<TokenType> const &b) {
	return ParserRef<TokenType>(new AlternativeParser<TokenType>(a, b));
}

template<typename TokenType>
inline ParserRef<TokenType> operator ~ (ParserRef<TokenType> a) {
	return ParserRef<TokenType>(new OptionalParser<TokenType>(a));
}

template<typename TokenType>
inline ParserRef<TokenType> operator * (ParserRef<TokenType> a) {
	return ParserRef<TokenType>(new KleeneStarParser<TokenType>(a));
}

template<typename TokenType, typename SemanticAction>
inline ParserRef<TokenType> operator >>(TokenType token, SemanticAction semanticAction)
{
	typedef typename std::conditional<function_traits<SemanticAction>::arity == 0
			, SimpleParser<TokenType, SemanticAction>
			, TokenParser<TokenType, SemanticAction>>
			::type Parser;

	return ParserRef<TokenType>(new Parser(token, semanticAction));
}

template<typename TokenType, typename Transformation>
inline ParserRef<TokenType> operator >>(ParserRef<TokenType> parser, Transformation transformation)
{
	return ParserRef<TokenType>(new TransformingParser<TokenType, Transformation>(parser, transformation));
}

template<typename TokenType>
inline ParserRef<TokenType> operator & (TokenType const &token, ParserRef<TokenType> const &b) {
	return ParserRef<TokenType>(new ConcatenationParser<TokenType>(token >>
			[] (lexer::Token<TokenType> const &token) { return new TemporaryToken<TokenType>(token); }, b));
}

template<typename TokenType>
inline ParserRef<TokenType> operator & (ParserRef<TokenType> const &a, TokenType const &token) {
	return ParserRef<TokenType>(new ConcatenationParser<TokenType>(a, token >>
			[] (lexer::Token<TokenType> const &token) { return new TemporaryToken<TokenType>(token); }));
}

template<typename TokenType>
inline ParserRef<TokenType> operator & (TokenType const &token1, TokenType const &token2) {
	return ParserRef<TokenType>(new ConcatenationParser<TokenType>(
			token1 >> [] (lexer::Token<TokenType> const &token) { return new TemporaryToken<TokenType>(token); }
			, token2 >> [] (lexer::Token<TokenType> const &token) { return new TemporaryToken<TokenType>(token); })
			);
}

template<typename TokenType>
inline ParserRef<TokenType> operator - (TokenType const &token) {
	return token >> [] { return new TemporaryDiscardableNode(); };
}

template<typename TokenType>
inline ParserRef<TokenType> operator - (ParserRef<TokenType> const &parser) {
	return parser >> [] (QSharedPointer<ast::Node> node) {
			Q_UNUSED(node);
			return wrap(new TemporaryDiscardableNode());
	};
}

}
}
}
