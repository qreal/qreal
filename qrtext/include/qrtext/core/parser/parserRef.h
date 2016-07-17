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
	ParserRef(const ParserRef<TokenType> &other) {
		mRef = other.mRef;
	}

	/// Assignment operator that assigns parsers themselves, not "external" pointers. Note that it has different
	/// semantics with copy constructor, so be cautious.
	ParserRef &operator =(const ParserRef<TokenType> &other) {
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
