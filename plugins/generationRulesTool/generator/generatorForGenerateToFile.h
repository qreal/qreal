#pragma once

#include <QtCore/QString>
#include <QtCore/QSharedPointer>

#include <qrgui/plugins/interpretedPluginInterface/interpretedPluginInterface.h>
#include <qrgui/plugins/pluginManager/editorManagerInterface.h>

#include "ast/generateToFile.h"

#include "commonInfo/variablesTable.h"
#include "commonInfo/generatorConfigurer.h"
#include "commonInfo/scopeInfo.h"

namespace generationRules {
namespace generator {

/// Class that generates code for generateToFile node.
class GeneratorForGenerateToFile
{
public:
	/// Returns generated string.
	/// @param generateToFileNode - node to generate code for.
	/// @param generatorConfigurer - information about model and metamodel.
	/// @param scopeInfo - information about current scope.
	static QString generatedResult(const QSharedPointer<simpleParser::ast::GenerateToFile> &generateToFileNode
			, const GeneratorConfigurer &generatorConfigurer
			, ScopeInfo &scopeInfo
			);

private:
	static void writeToFile(
			const QString &resultOfGeneration
			, const QString &fileName
			, const QString &pathToCode);
};

}
}
