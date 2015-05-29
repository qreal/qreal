#pragma once

#include <QtCore/QString>
#include <QtCore/QSharedPointer>

#include <qrgui/plugins/interpretedPluginInterface/interpretedPluginInterface.h>
#include <qrgui/plugins/pluginManager/editorManagerInterface.h>

#include "ast/generateToFile.h"

#include "commonInfo/variablesTable.h"
#include "commonInfo/generatorConfigurer.h"

namespace generationRules {
namespace generator {

class GeneratorForGenerateToFile
{
public:
	static QString generatedResult(QSharedPointer<simpleParser::ast::GenerateToFile> generateToFileNode
			, GeneratorConfigurer generatorConfigurer
			);

private:
	static void writeToFile(
			const QString &resultOfGeneration
			, const QString &fileName
			, const QString &pathToCode);
};

}
}
