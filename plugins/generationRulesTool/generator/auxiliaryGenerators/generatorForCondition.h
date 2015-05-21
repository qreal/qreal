#pragma once

#include <QtCore/QString>
#include <QtCore/QSharedPointer>

#include "ast/condition.h"

#include "generator/generatorConfigurer.h"

namespace generationRules {
namespace generator {

class ConditionGenerator
{
public:
	static bool conditionSatisfied(QSharedPointer<simpleParser::ast::Condition> conditionNode
			, GeneratorConfigurer generatorConfigurer);
};

}
}
