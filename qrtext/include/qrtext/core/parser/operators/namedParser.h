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
