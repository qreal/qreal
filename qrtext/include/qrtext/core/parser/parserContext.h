/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

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
	void reportError(QSharedPointer<ast::Node> const &node, const QString &message)
	{
		report(node ? node->start() : Connection(), message, Severity::error);
	}

	/// Puts error connected to given token to error stream.
	void reportError(Token<TokenType> const &token, const QString &message)
	{
		report(token.range().start(), message, Severity::error);
	}

	/// Puts error connected to current token to error stream.
	void reportError(const QString &message)
	{
		report(mTokenStream.next().range().start(), message, Severity::error);
	}

	/// Puts internal error connected to current token to error stream.
	void reportInternalError(const QString &message)
	{
		report(mTokenStream.next().range().start(), message, Severity::internalError);
	}

private:
	void report(const Connection &connection, const QString &message, Severity severity)
	{
		mErrors << Error(connection, message, ErrorType::syntaxError, severity);
	}

	QList<Error> &mErrors;
	TokenStream<TokenType> const &mTokenStream;
};

}
}
