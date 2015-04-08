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

#include <QtCore/QDebug>

#include "qrtext/core/parser/parserRef.h"
#include "qrtext/core/parser/operators/parserInterface.h"

namespace qrtext {
namespace core {

/// Parser that only has a name, to be seen in stack trace to identify our current position in recursive descent.
template<typename TokenType>
class NamedParser : public ParserInterface<TokenType>
{
public:
	/// Constructor.
	/// @param parser - internal parser, to do actual work.
	/// @param name - name of a production, for debugging purposes.
	NamedParser(const ParserRef<TokenType> &parser, const QString &name)
		: mParser(parser), mName(name)
	{
	}

	QSharedPointer<ast::Node> parse(TokenStream<TokenType> &tokenStream
			, ParserContext<TokenType> &parserContext) const override
	{
		return mParser->parse(tokenStream, parserContext);
	}

	QSet<TokenType> first() const override
	{
		return mParser->first();
	}

private:
	/// Nested parser that does actual parsing.
	ParserRef<TokenType> mParser;

	/// Name of a production, for easier debugging.
	QString mName;
};

}
}
