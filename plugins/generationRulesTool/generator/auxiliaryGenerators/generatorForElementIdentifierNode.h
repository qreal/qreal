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
#include <QtCore/QSharedPointer>

#include <qrgui/plugins/interpretedPluginInterface/interpretedPluginInterface.h>

#include "ast/elementIdentifier.h"
#include "ast/identifier.h"
#include "ast/node.h"

#include "generator/commonInfo/variablesTable.h"
#include "generator/commonInfo/generatorConfigurer.h"
#include "generator/commonInfo/scopeInfo.h"

namespace generationRules {
namespace generator {

/// Class that generates result for elementIdentifier node.
class GeneratorForElementIdentifierNode
{
public:
	/// Returns id of element identifier.
	/// @param elementIdentifierNode - element identifier node.
	/// @param generatorConfigurer - information about model and metamodel.
	/// @param scopeInfo - information about current scope.
	static qReal::Id neededElementId(const QSharedPointer<simpleParser::ast::ElementIdentifier> &elementIdentifierNode
			, const GeneratorConfigurer &generatorConfigurer
			, ScopeInfo &scopeInfo
			);
};

}
}
