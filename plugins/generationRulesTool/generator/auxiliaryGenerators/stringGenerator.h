#pragma once

#include <QtCore/QString>
#include <QtCore/QSharedPointer>

#include "generator/generatorConfigurer.h"

#include "ast/string.h"

namespace generationRules {
namespace generator {

class StringGenerator
{
public:
	static QString generatedString(QSharedPointer<simpleParser::ast::String> stringNode
			, GeneratorConfigurer generatorCongifurer);
};

}
}
