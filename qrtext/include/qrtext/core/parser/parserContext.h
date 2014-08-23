#pragma once

#include <QtCore/QList>
#include <QtCore/QSharedPointer>

#include "qrtext/core/error.h"
#include "qrtext/core/ast/node.h"
#include "qrtext/core/error.h"
#include "qrtext/core/parser/tokenStream.h"

namespace qrtext {
namespace core {

template<typename TokenType>
class ParserContext
{
public:
	ParserContext(QList<Error> &errors, TokenStream<TokenType> const &tokenStream)
		: mErrors(errors), mTokenStream(tokenStream)
	{
	}

	void reportError(QSharedPointer<ast::Node> const &node, QString const &message)
	{
		report(node ? node->start() : Connection(), message, Severity::error);
	}

	void reportError(Token<TokenType> const &token, QString const &message)
	{
		report(token.range().start(), message, Severity::error);
	}

	void reportError(QString const &message)
	{
		report(mTokenStream.next().range().start(), message, Severity::error);
	}

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
