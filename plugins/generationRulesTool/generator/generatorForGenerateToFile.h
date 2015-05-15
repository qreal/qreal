#pragma once

#include <QtCore/QString>
#include <QtCore/QSharedPointer>

#include <qrgui/plugins/interpretedPluginInterface/interpretedPluginInterface.h>
#include <qrgui/plugins/pluginManager/editorManagerInterface.h>

#include "ast/generateToFile.h"

#include "variablesTable.h"
#include "generatorConfigurer.h"

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

	static qReal::Id idInMetamodel(qReal::EditorManagerInterface *editorManagerInterface
			, const QString &elementName
			, const qReal::Id &diagramId);
};

}
}
