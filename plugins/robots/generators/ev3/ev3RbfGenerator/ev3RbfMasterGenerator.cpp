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

#include "ev3RbfMasterGenerator.h"

#include <QtCore/QDir>

#include <qrtext/languageToolboxInterface.h>
#include <qrutils/stringUtils.h>
#include <generatorBase/parts/initTerminateCodeGenerator.h>
#include <generatorBase/gotoControlFlowGenerator.h>
#include <generatorBase/parts/subprograms.h>
#include <generatorBase/parts/threads.h>
#include <generatorBase/parts/variables.h>

#include "ev3RbfGeneratorCustomizer.h"
#include "lua/ev3LuaProcessor.h"

using namespace ev3::rbf;
using namespace generatorBase;

using namespace qReal;

Ev3RbfMasterGenerator::Ev3RbfMasterGenerator(const qrRepo::RepoApi &repo
		, qReal::ErrorReporterInterface &errorReporter
		, const utils::ParserErrorReporter &parserErrorReporter
		, const kitBase::robotModel::RobotModelManagerInterface &robotModelManager
		, qrtext::LanguageToolboxInterface &textLanguage
		, const qReal::Id &diagramId
		, const QString &generatorName)
	: Ev3MasterGeneratorBase(repo, errorReporter, parserErrorReporter
			, robotModelManager, textLanguage, diagramId, generatorName)
	, mLuaProcessorInstance(nullptr)
{
}

void Ev3RbfMasterGenerator::initialize()
{
	Ev3MasterGeneratorBase::initialize();
	if (mLuaProcessorInstance) {
		mLuaProcessorInstance->configure(mCustomizer->factory()->variables()
				, dynamic_cast<Ev3RbfGeneratorFactory *>(mCustomizer->factory()));
	}
}

QString Ev3RbfMasterGenerator::generate(const QString &indentString)
{
	if (mDiagram.isNull()) {
		mErrorReporter.addCritical(QObject::tr("There is no opened diagram"));
		return QString();
	}

	beforeGeneration();
	if (!QDir(mProjectDir).exists()) {
		QDir().mkpath(mProjectDir);
	}

	mTextLanguage.clear();
	mCustomizer->factory()->setMainDiagramId(mDiagram);

	for (parts::InitTerminateCodeGenerator *generator : mCustomizer->factory()->initTerminateGenerators()) {
		generator->reinit();
	}

	QString mainCode;
	const semantics::SemanticTree * const gotoMainControlFlow = mGotoControlFlowGenerator->generate();
	if (gotoMainControlFlow) {
		mainCode = gotoMainControlFlow->toString(1, indentString);
		bool const gotoSubprogramsResult = mCustomizer->factory()
				->subprograms()->generate(mGotoControlFlowGenerator, indentString);
		if (!gotoSubprogramsResult) {
			mainCode = QString();
		}
	}

	if (mainCode.isEmpty()) {
		return QString();
	}

	QString resultCode = readTemplate("main.t");
	resultCode.replace("@@SUBPROGRAMS_FORWARDING@@", mCustomizer->factory()->subprograms()->forwardDeclarations());
	resultCode.replace("@@SUBPROGRAMS@@", mCustomizer->factory()->subprograms()->implementations());
	resultCode.replace("@@THREADS_FORWARDING@@", mCustomizer->factory()->threads().generateDeclarations());
	resultCode.replace("@@THREADS@@", mCustomizer->factory()->threads().generateImplementations(indentString));
	resultCode.replace("@@MAIN_CODE@@", mainCode);
	resultCode.replace("@@CONSTANTS_INITIALIZATION@@", utils::StringUtils::addIndent(
			mLuaProcessorInstance->constantsValuation(), 1, indentString));
	resultCode.replace("@@INITHOOKS@@", utils::StringUtils::addIndent(
			mCustomizer->factory()->initCode(), 1, indentString));
	resultCode.replace("@@TERMINATEHOOKS@@", utils::StringUtils::addIndent(
			mCustomizer->factory()->terminateCode(), 1, indentString));
	resultCode.replace("@@USERISRHOOKS@@", utils::StringUtils::addIndent(
			mCustomizer->factory()->isrHooksCode(), 1, indentString));
	resultCode.replace("@@VARIABLES@@", utils::StringUtils::addIndent(
			mCustomizer->factory()->variables()->generateVariableString(), 1, "\t"));
	// This will remove too many empty lines
	resultCode.replace(QRegExp("\n(\n)+"), "\n\n");

	processGeneratedCode(resultCode);

	const QString pathToOutput = targetPath();
	outputCode(pathToOutput, resultCode);

	afterGeneration();

	return pathToOutput;
}

QString Ev3RbfMasterGenerator::targetPath()
{
	return QString("%1/%2.lms").arg(mProjectDir, mProjectName);
}

bool Ev3RbfMasterGenerator::supportsGotoGeneration() const
{
	return true;
}

generatorBase::lua::LuaProcessor *Ev3RbfMasterGenerator::createLuaProcessor()
{
	mLuaProcessorInstance = new lua::Ev3LuaProcessor(mErrorReporter, mTextLanguage
			, mParserErrorReporter, this);
	return mLuaProcessorInstance;
}

GeneratorCustomizer *Ev3RbfMasterGenerator::createCustomizer()
{
	return new Ev3RbfGeneratorCustomizer(mRepo, mErrorReporter
			, mRobotModelManager, *createLuaProcessor(), mGeneratorName);
}
