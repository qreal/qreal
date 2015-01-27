#pragma once

#include <QtCore/QDebug>

#include "qrtext/core/parser/parserRef.h"
#include "qrtext/core/parser/operators/parserInterface.h"

namespace qrtext {
namespace core {

template<typename TokenType>
class NamedParser : public ParserInterface<TokenType>
{
public:
	/// Constructor.
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
	ParserRef<TokenType> mParser;
	QString mName;
};

}
}
