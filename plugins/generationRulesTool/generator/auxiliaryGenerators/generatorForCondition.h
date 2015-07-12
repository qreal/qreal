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

#include "ast/condition.h"

#include "generator/commonInfo/generatorConfigurer.h"
#include "generator/commonInfo/scopeInfo.h"

namespace generationRules {
namespace generator {

/// Class that generates result for condition node.
class ConditionGenerator
{
public:
	/// Returns true if condition from conditionNode is satisfied, false otherwise.
	/// @param conditionNode - node with condition to check.
	/// @param generatorConfigurer - information about model and metamodel.
	/// @param scopeInfo - information about current scope.
	static bool conditionSatisfied(const QSharedPointer<simpleParser::ast::Condition> &conditionNode
			, const GeneratorConfigurer &generatorConfigurer
			, ScopeInfo &scopeInfo);
};

}
}
