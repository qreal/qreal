#pragma once

#include <QtCore/QString>
#include <QtCore/QSharedPointer>

#include <qrgui/plugins/interpretedPluginInterface/interpretedPluginInterface.h>

#include "ast/node.h"

namespace generationRules {
namespace generator {

class CommonGenerator
{
public:
	static QString generatedResult(QSharedPointer<simpleParser::ast::Node> node
			, qrRepo::LogicalRepoApi *metamodelRepoApi
			, const qReal::Id elementId = qReal::Id()
			);
};
}
}
