/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

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
