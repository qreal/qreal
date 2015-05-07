#pragma once

#include <QtCore/QString>
#include <QtCore/QSharedPointer>

#include "ast/elementIdentifier.h"

#include "GeneratorConfigurer.h"

namespace generationRules {
namespace generator {

class TypeQualifier
{
public:
	static QString elementIdentifierType(QSharedPointer<simpleParser::ast::ElementIdentifier> elementIdentifier
		, GeneratorConfigurer generatorConfigurer);
};

}
}
