/* Copyright 2016 CyberTech Labs Ltd.
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

#include "trikPythonMasterGenerator.h"

#include "trikPythonControlFlowValidator.h"

using namespace trik::python;

TrikPythonMasterGenerator::TrikPythonMasterGenerator(const qrRepo::RepoApi &repo
		, qReal::ErrorReporterInterface &errorReporter
		, const utils::ParserErrorReporter &parserErrorReporter
		, const kitBase::robotModel::RobotModelManagerInterface &robotModelManager
		, qrtext::LanguageToolboxInterface &textLanguage
		, const qReal::Id &diagramId
		, const QStringList &pathsToTemplates)
	: TrikMasterGeneratorBase(repo, errorReporter, parserErrorReporter, robotModelManager, textLanguage
			, diagramId, pathsToTemplates)
{
}

QString TrikPythonMasterGenerator::targetPath()
{
	return QString("%1/%2.py").arg(mProjectDir, mProjectName);
}

bool TrikPythonMasterGenerator::supportsGotoGeneration() const
{
	return false;
}

bool TrikPythonMasterGenerator::supportsSwitchUnstableToBreaks() const
{
	return false;
}

generatorBase::PrimaryControlFlowValidator *TrikPythonMasterGenerator::createValidator()
{
	return new TrikPythonControlFlowValidator(mRepo, mErrorReporter, *mCustomizer, this);
}
