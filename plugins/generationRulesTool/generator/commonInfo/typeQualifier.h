#pragma once

#include <QtCore/QString>
#include <QtCore/QSharedPointer>

#include "ast/elementIdentifier.h"

#include "generatorConfigurer.h"

namespace generationRules {
namespace generator {

/// Class that provides information about element type.
class TypeQualifier
{
public:
	/// Returns element type.
	/// @param elementIdentifier - element identifier node.
	/// @param generatorConfigurer - information about model and metamodel.
	static QString elementIdentifierType(const QSharedPointer<simpleParser::ast::ElementIdentifier> &elementIdentifier
		, const GeneratorConfigurer &generatorConfigurer);
};

}
}