#pragma once

#include <QtCore/QString>
#include <QtCore/QSharedPointer>

#include <qrgui/plugins/interpretedPluginInterface/interpretedPluginInterface.h>

#include "ast/complexIdentifier.h"

namespace generationRules {
namespace generator {

class GeneratorForComplexIdentifierNode
{
public:
	static QString generatedResult(QSharedPointer<simpleParser::ast::ComplexIdentifier> complexIdentifierNode
			, qrRepo::LogicalRepoApi *metamodelRepoApi
			, qReal::Id const elementId
			);
};

}
}
