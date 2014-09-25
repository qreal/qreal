#pragma once

#include <QtCore/QSharedPointer>

#include "qrtext/core/parser/operators/parserInterface.h"

namespace qrtext {
namespace core {

/// Reference to a parser. It actually is a pointer to a pointer, so the parser can change but all ParserRefs will
/// remain valid and updated. Used to allow recursive productions.
template<typename TokenType>
class ParserRef
{
public:
	/// Constructor, which creates empty ParserRef.
	ParserRef()
		: mRef(QSharedPointer<QSharedPointer<ParserInterface<TokenType>>>(
			new QSharedPointer<ParserInterface<TokenType>>()))
	{
	}

	/// Constructor which creates ParserRef to given parser.
	ParserRef(ParserInterface<TokenType> * parser)
		: mRef(QSharedPointer<QSharedPointer<ParserInterface<TokenType>>>(
			new QSharedPointer<ParserInterface<TokenType>>(parser)))
	{
	}

	/// Copy constructor.
	ParserRef(ParserRef<TokenType> const &other) {
		mRef = other.mRef;
	}

	/// Assignment operator that assigns parsers themselves, not "external" pointers. Note that it has different
	/// semantics with copy constructor, so be cautious.
	ParserRef &operator =(ParserRef<TokenType> const &other) {
		*mRef = *(other.mRef);
		return *this;
	}

	/// Returns parser itself.
	ParserInterface<TokenType> const *operator ->() const {
		return mRef->data();
	}

	/// Returns "internal" pointer to parser.
	QSharedPointer<ParserInterface<TokenType>> parser() const {
		return *mRef;
	}

private:
	QSharedPointer<QSharedPointer<ParserInterface<TokenType>>> mRef;
};

}
}
