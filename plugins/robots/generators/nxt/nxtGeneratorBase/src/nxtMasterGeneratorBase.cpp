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

#include "nxtGeneratorBase/nxtMasterGeneratorBase.h"

#include "nxtGeneratorCustomizer.h"
#include "nxtGeneratorBase/parts/images.h"

using namespace nxt;

NxtMasterGeneratorBase::NxtMasterGeneratorBase(const qrRepo::RepoApi &repo
		, qReal::ErrorReporterInterface &errorReporter
		, const utils::ParserErrorReporter &parserErrorReporter
		, const kitBase::robotModel::RobotModelManagerInterface &robotModelManager
		, qrtext::LanguageToolboxInterface &textLanguage
		, const qReal::Id &diagramId
		, const QString &generatorName)
	: MasterGeneratorBase(repo, errorReporter, robotModelManager, textLanguage, parserErrorReporter, diagramId)
	, mGeneratorName(generatorName)
{
}

generatorBase::GeneratorCustomizer *NxtMasterGeneratorBase::createCustomizer()
{
	return new NxtGeneratorCustomizer(mRepo, mErrorReporter, mRobotModelManager, *createLuaProcessor(), mGeneratorName);
}

void NxtMasterGeneratorBase::beforeGeneration()
{
	static_cast<NxtGeneratorFactory *>(mCustomizer->factory())->images().reinit();
}

void NxtMasterGeneratorBase::processGeneratedCode(QString &code)
{
	code.replace("@@BMP_FILES@@", static_cast<NxtGeneratorFactory *>(mCustomizer->factory())->images().generate());
}

void NxtMasterGeneratorBase::afterGeneration()
{
	saveImages(mProjectDir);
}

void NxtMasterGeneratorBase::saveImages(const QString &projectDir)
{
	QMap<QString, QImage> &images = static_cast<NxtGeneratorFactory *>(mCustomizer->factory())->images().bmpFiles();
	for (const QString &fileName : images.keys()) {
		images[fileName].save(projectDir + '/' + fileName + ".bmp", "BMP", -1);
	}
}
