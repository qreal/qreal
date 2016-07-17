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

#include <qrtext/core/parser/precedenceTable.h>
#include <qrtext/core/ast/node.h>

#include "generatorBase/robotsGeneratorDeclSpec.h"

namespace generatorBase {
namespace lua {

/// Provides info about precedencies and associativity of the operators of
/// the target language (i.e. the language we generate to). Converts thus
/// lua ast nodes to target language`s operations.
class ROBOTS_GENERATOR_EXPORT PrecedenceConverterInterface
{
public:
	virtual ~PrecedenceConverterInterface() {}

	/// Returns a precedence of the operator represented by the given AST node.
	/// Bigger precedence numbers correspond to higher precedence.
	virtual int precedence(const QSharedPointer<qrtext::core::ast::Node> &node) = 0;

	/// Returns the associativity of the binary operator represented by the given AST node.
	virtual qrtext::core::Associativity associativity(const QSharedPointer<qrtext::core::ast::Node> &node) = 0;
};

}
}

