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

#include "nxtRussianCMasterGenerator.h"

using namespace nxt::russianC;

NxtRussianCMasterGenerator::NxtRussianCMasterGenerator(const qrRepo::RepoApi &repo
		, qReal::ErrorReporterInterface &errorReporter
		, const utils::ParserErrorReporter &parserErrorReporter
		, const kitBase::robotModel::RobotModelManagerInterface &robotModelManager
		, qrtext::LanguageToolboxInterface &textLanguage
		, const qReal::Id &diagramId
		, const QString &generatorName)
	: NxtMasterGeneratorBase(repo, errorReporter, parserErrorReporter
			, robotModelManager, textLanguage, diagramId, generatorName)
{
}

QString NxtRussianCMasterGenerator::targetPath()
{
	return QString::fromUtf8("%1/%2.си").arg(mProjectDir, mProjectName);
}

bool NxtRussianCMasterGenerator::supportsGotoGeneration() const
{
	return false;
}
