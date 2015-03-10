#pragma once

#include <QtCore/QSharedPointer>
#include <QtCore/QString>

#include <qrgui/plugins/interpretedPluginInterface/interpretedPluginInterface.h>

#include "ast/program.h"

namespace generationRules {
namespace generator {

class GeneratorForProgramNode
{
public:
	static QString generatedResult(QSharedPointer<simpleParser::ast::Program> programNode
			, qrRepo::LogicalRepoApi *metamodelRepoApi
			, qrRepo::RepoControlInterface *modelRepo
			, qReal::LogicalModelAssistInterface *logicalModelInterface
			, qReal::Id const elementId
			);
};

}
}
