#pragma once

#include <QtCore/QString>
#include <QtCore/QSharedPointer>

#include "ast/condition.h"

#include "generator/commonInfo/generatorConfigurer.h"

namespace generationRules {
namespace generator {

/// Class that generates result for condition node.
class ConditionGenerator
{
public:
	/// Returns true if condition from conditionNode is satisfied, false otherwise.
	/// @param conditionNode - node with condition to check.
	/// @generatorConfigurer - information about model and metamodel.
	static bool conditionSatisfied(const QSharedPointer<simpleParser::ast::Condition> &conditionNode
			, GeneratorConfigurer generatorConfigurer);
};

}
}
