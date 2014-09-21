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

/// Operator to build concatenation rules. A ::= B C is described as ParserRef a = b & c;
template<typename TokenType>
inline ParserRef<TokenType> operator & (ParserRef<TokenType> const &a, ParserRef<TokenType> const &b) {
	return ParserRef<TokenType>(new ConcatenationParser<TokenType>(a, b));
}

/// Operator to build alternative rules. A ::= B | C is described as ParserRef a = b | c;
template<typename TokenType>
inline ParserRef<TokenType> operator | (ParserRef<TokenType> const &a, ParserRef<TokenType> const &b) {
	return ParserRef<TokenType>(new AlternativeParser<TokenType>(a, b));
}

/// Operator to build optional rules. A ::= B? is described as ParserRef a = ~b;
template<typename TokenType>
inline ParserRef<TokenType> operator ~ (ParserRef<TokenType> const &a) {
	return ParserRef<TokenType>(new OptionalParser<TokenType>(a));
}

/// Operator to build Kleene star rules. A ::= B* is described as ParserRef a = *b;
template<typename TokenType>
inline ParserRef<TokenType> operator * (ParserRef<TokenType> const &a) {
	return ParserRef<TokenType>(new KleeneStarParser<TokenType>(a));
}

/// Operator to attach semantic rule to a token. A ::= t { ... } is described as ParserRef a = t >> []() { ... };
template<typename TokenType, typename SemanticAction>
inline ParserRef<TokenType> operator >>(TokenType token, SemanticAction const &semanticAction)
{
	typedef typename std::conditional<function_traits<SemanticAction>::arity == 0
			, SimpleParser<TokenType, SemanticAction>
			, TokenParser<TokenType, SemanticAction>>
			::type Parser;

	return ParserRef<TokenType>(new Parser(token, semanticAction));
}

/// Operator to attach semantic rule to a parser. A ::= B { ... } is described as ParserRef a = b >> []() { ... };
template<typename TokenType, typename Transformation>
inline ParserRef<TokenType> operator >>(ParserRef<TokenType> const &parser, Transformation const &transformation)
{
	return ParserRef<TokenType>(new TransformingParser<TokenType, Transformation>(parser, transformation));
}

/// Syntactic sugar operator for concatenation of tokens with parsers.
template<typename TokenType>
inline ParserRef<TokenType> operator & (TokenType const &token, ParserRef<TokenType> const &b) {
	return ParserRef<TokenType>(new ConcatenationParser<TokenType>(token >>
			[] (Token<TokenType> const &token) { return new TemporaryToken<TokenType>(token); }, b));
}

/// Syntactic sugar operator for concatenation of parsers with tokens.
template<typename TokenType>
inline ParserRef<TokenType> operator & (ParserRef<TokenType> const &a, TokenType const &token) {
	return ParserRef<TokenType>(new ConcatenationParser<TokenType>(a, token >>
			[] (Token<TokenType> const &token) { return new TemporaryToken<TokenType>(token); }));
}

/// Syntactic sugar operator for concatenation of tokens and tokens.
template<typename TokenType>
inline ParserRef<TokenType> operator & (TokenType const &token1, TokenType const &token2) {
	return ParserRef<TokenType>(new ConcatenationParser<TokenType>(
			token1 >> [] (Token<TokenType> const &token) { return new TemporaryToken<TokenType>(token); }
			, token2 >> [] (Token<TokenType> const &token) { return new TemporaryToken<TokenType>(token); })
			);
}

/// Operator to discard a token, used to ignore a token from a token stream. For example, A ::= '(' B ')' may be
/// represented as ParserRef a = -TokenType::openingBracket & b & -TokenType::closingBracket, in this example
/// parser A will directly return value of B. Connections will be lost.
template<typename TokenType>
inline ParserRef<TokenType> operator - (TokenType const &token) {
	return token >> [] { return new TemporaryDiscardableNode(); };
}

/// Operator to discard a result of parser, used to ignore entire subtree. Connections from ignored subtree
/// will be lost.
template<typename TokenType>
inline ParserRef<TokenType> operator - (ParserRef<TokenType> const &parser) {
	return parser >> [] (QSharedPointer<ast::Node> node) {
			Q_UNUSED(node);
			return wrap(new TemporaryDiscardableNode());
	};
}

}
}
