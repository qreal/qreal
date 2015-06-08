#pragma once

#include <QtCore/QString>
#include <QtCore/QSharedPointer>

#include "generator/commonInfo/generatorConfigurer.h"

#include "ast/string.h"

namespace generationRules {
namespace generator {

/// Class that generates string (for string node).
class StringGenerator
{
public:
	/// Returns generated string.
	/// @param stringNode - string node.
	/// @param generatorConfigurer - information about model and metamodel.
	static QString generatedString(const QSharedPointer<simpleParser::ast::String> &stringNode
			, GeneratorConfigurer generatorConfigurer);
};

}
}
