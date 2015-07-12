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

#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtCore/QSharedPointer>

#include <qrgui/plugins/interpretedPluginInterface/interpretedPluginInterface.h>

#include "ast/complexIdentifier.h"
#include "ast/identifier.h"
#include "ast/node.h"

#include "commonInfo/variablesTable.h"
#include "commonInfo/generatorConfigurer.h"
#include "commonInfo/scopeInfo.h"

namespace generationRules {
namespace generator {

/// Class that generates string for complexIdentifier node.
class GeneratorForComplexIdentifierNode
{
public:
	/// @param complexIdentifier - identifier node.
	/// @param generatorConfigurer - all information about generator.
	/// @param scopeInfo - information about current scope.
	static QVariant generatedResult(const QSharedPointer<simpleParser::ast::ComplexIdentifier> &complexIdentifierNode
			, const GeneratorConfigurer &generatorConfigurer
			, ScopeInfo &scopeInfo);
};

}
}
