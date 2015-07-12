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

#include <qrgui/plugins/interpretedPluginInterface/interpretedPluginInterface.h>

#include <QtCore/QSharedPointer>
#include <QtCore/QString>

#include "ast/generator.h"

#include "commonInfo/variablesTable.h"
#include "commonInfo/generatorConfigurer.h"
#include "commonInfo/scopeInfo.h"

namespace generationRules {
namespace generator {

/// Class that generates code for generator node.
class GeneratorForGeneratorNode
{
public:
	/// Returns generated string.
	/// @param generatorNode - node to generate code for.
	/// @param generatorConfigurer - information about model and metamodel.
	/// @param scopeInfo - information about current scope.
	static QString generatedResult(
			const QSharedPointer<simpleParser::ast::Generator> &generatorNode
			, const GeneratorConfigurer &generatorConfigurer
			, ScopeInfo &scopeInfo);
};

}
}
