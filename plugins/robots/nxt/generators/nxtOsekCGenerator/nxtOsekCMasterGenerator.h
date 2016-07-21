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

#include <nxtGeneratorBase/nxtMasterGeneratorBase.h>

namespace nxt {
namespace osekC {

class NxtOsekCMasterGenerator : public NxtMasterGeneratorBase
{
public:
	NxtOsekCMasterGenerator(const qrRepo::RepoApi &repo
			, qReal::ErrorReporterInterface &errorReporter
			, const utils::ParserErrorReporter &parserErrorReporter
			, const kitBase::robotModel::RobotModelManagerInterface &robotModelManager
			, qrtext::LanguageToolboxInterface &textLanguage
			, const qReal::Id &diagramId
			, const QString &generatorName);

	void generateOilAndMakeFiles();

protected:
	QString targetPath() override;
	bool supportsGotoGeneration() const override;

	void afterGeneration() override;

private:
	void generateOilFile(const QString &projectName, const QString &projectDir);
	QString generateOilTask(const QString &taskName);
	void generateMakeFile(const QString &projectName, const QString &projectDir);
};

}
}
