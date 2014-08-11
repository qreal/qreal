#pragma once

#include <QtCore/QSharedPointer>

#include "textLanguageParser/details/parsers/parserInterface.h"

namespace textLanguageParser {
namespace details {

class ParserRef
{
public:
	ParserRef()
		: mRef(QSharedPointer<QSharedPointer<ParserInterface>>(new QSharedPointer<ParserInterface>()))
	{
	}

	ParserRef(ParserInterface * parser)
		: mRef(QSharedPointer<QSharedPointer<ParserInterface>>(new QSharedPointer<ParserInterface>(parser)))
	{
	}

	ParserRef(ParserRef const &other) {
		mRef = other.mRef;
	}

	ParserRef &operator =(ParserRef const &other) {
		*mRef = *(other.mRef);
		return *this;
	}

	ParserInterface const *operator ->() const {
		return mRef->data();
	}

private:
	QSharedPointer<QSharedPointer<ParserInterface>> mRef;
};

}
}
