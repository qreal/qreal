#pragma once

#include <QtCore/QList>
#include <QtCore/QSharedPointer>

#include "qrtext/core/error.h"
#include "qrtext/core/ast/node.h"
#include "qrtext/core/error.h"
#include "qrtext/core/parser/tokenStream.h"

namespace qrtext {
namespace core {

/// Holds context of a parser, passed to every parser in grammar. Now is used for error reporting.
template<typename TokenType>
class ParserContext
{
public:
	/// Constructor. Gets reference to error stream and stream of tokens to get connection from.
	ParserContext(QList<Error> &errors, TokenStream<TokenType> const &tokenStream)
		: mErrors(errors), mTokenStream(tokenStream)
	{
	}

	/// Puts error connected to given node to error stream.
	void reportError(QSharedPointer<ast::Node> const &node, QString const &message)
	{
		report(node ? node->start() : Connection(), message, Severity::error);
	}

	/// Puts error connected to given token to error stream.
	void reportError(Token<TokenType> const &token, QString const &message)
	{
		report(token.range().start(), message, Severity::error);
	}

	/// Puts error connected to current token to error stream.
	void reportError(QString const &message)
	{
		report(mTokenStream.next().range().start(), message, Severity::error);
	}

	/// Puts internal error connected to current token to error stream.
	void reportInternalError(QString const &message)
	{
		report(mTokenStream.next().range().start(), message, Severity::internalError);
	}

private:
	void report(Connection const &connection, QString const &message, Severity severity)
	{
		mErrors << Error(connection, message, ErrorType::syntaxError, severity);
	}

	QList<Error> &mErrors;
	TokenStream<TokenType> const &mTokenStream;
};

}
}
