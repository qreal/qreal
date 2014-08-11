#pragma once

#include <QtCore/QList>
#include <QtCore/QSharedPointer>

#include "textLanguageParser/parserError.h"
#include "textLanguageParser/ast/nodes/node.h"
#include "textLanguageParser/details/token.h"
#include "textLanguageParser/details/tokenStream.h"

namespace textLanguageParser {
namespace details {

class ParserContext
{
public:
	ParserContext(QList<ParserError> &errors, TokenStream const &tokenStream)
		: mErrors(errors), mTokenStream(tokenStream)
	{
	}

	void reportError(QSharedPointer<ast::Node> const &node, QString const &message)
	{
		report(node ? node->start() : ast::Connection(), message, Severity::error);
	}

	void reportError(Token const &token, QString const &message)
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
	void report(ast::Connection const &connection, QString const &message, Severity severity)
	{
		mErrors << ParserError(connection, message, ErrorType::syntaxError, severity);
	}

	QList<ParserError> &mErrors;
	TokenStream const &mTokenStream;
};

}
}
