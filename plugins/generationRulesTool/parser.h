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

#include <QtCore/QScopedPointer>

#include <qrtext/core/parser/parser.h>
#include <qrtext/core/parser/parserRef.h>
#include <qrtext/core/parser/operators/parserCombinators.h>

#include "tokenTypes.h"

namespace simpleParser {
class Parser : public qrtext::core::Parser<TokenTypes>
{
public:
	/// Constructor.
	/// @param errors - error stream to report errors to.
	explicit Parser(QList<qrtext::core::Error> &errors);

private:
	/// Returns parser for grammar to be called by parent.
	QSharedPointer<qrtext::core::ParserInterface<TokenTypes>> grammar();
};

}
