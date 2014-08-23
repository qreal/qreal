#pragma once

#include <QtCore/QSharedPointer>

#include "qrtext/core/parser/operators/parserInterface.h"

namespace qrtext {
namespace core {

template<typename TokenType>
class ParserRef
{
public:
	ParserRef()
		: mRef(QSharedPointer<QSharedPointer<ParserInterface<TokenType>>>(
			new QSharedPointer<ParserInterface<TokenType>>()))
	{
	}

	ParserRef(ParserInterface<TokenType> * parser)
		: mRef(QSharedPointer<QSharedPointer<ParserInterface<TokenType>>>(
			new QSharedPointer<ParserInterface<TokenType>>(parser)))
	{
	}

	ParserRef(ParserRef<TokenType> const &other) {
		mRef = other.mRef;
	}

	ParserRef &operator =(ParserRef<TokenType> const &other) {
		*mRef = *(other.mRef);
		return *this;
	}

	ParserInterface<TokenType> const *operator ->() const {
		return mRef->data();
	}

	QSharedPointer<ParserInterface<TokenType>> parser() const {
		return *mRef;
	}

private:
	QSharedPointer<QSharedPointer<ParserInterface<TokenType>>> mRef;
};

}
}
