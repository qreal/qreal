/* Copyright 2007-2015 QReal Research Group, Dmitry Mordvinov
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

#include "generatorBase/masterGeneratorBase.h"

#include <QtCore/QDir>

#include <qrutils/outFile.h>
#include <qrutils/fileSystemUtils.h>
#include <qrutils/stringUtils.h>
#include <qrtext/languageToolboxInterface.h>

#include "readableControlFlowGenerator.h"
#include "generatorBase/gotoControlFlowGenerator.h"
#include "generatorBase/lua/luaProcessor.h"
#include "generatorBase/parts/variables.h"
#include "generatorBase/parts/subprograms.h"
#include "generatorBase/parts/threads.h"
#include "generatorBase/parts/sensors.h"
#include "generatorBase/parts/initTerminateCodeGenerator.h"

using namespace generatorBase;
using namespace qReal;

MasterGeneratorBase::MasterGeneratorBase(const qrRepo::RepoApi &repo
		, ErrorReporterInterface &errorReporter
		, const kitBase::robotModel::RobotModelManagerInterface &robotModelManager
		, qrtext::LanguageToolboxInterface &textLanguage
		, const utils::ParserErrorReporter &parserErrorReporter
		, const Id &diagramId)
	: mRepo(repo)
	, mErrorReporter(errorReporter)
	, mRobotModelManager(robotModelManager)
	, mTextLanguage(textLanguage)
	, mDiagram(diagramId)
	, mParserErrorReporter(parserErrorReporter)
{
}

void MasterGeneratorBase::setProjectDir(const QFileInfo &fileInfo)
{
	mProjectName = fileInfo.completeBaseName();
	mProjectDir = fileInfo.absolutePath();
}

void MasterGeneratorBase::initialize()
{
	mCustomizer = createCustomizer();
	mCustomizer->factory()->initialize();
	setPathsToTemplates(mCustomizer->factory()->pathsToTemplates());

	mValidator = createValidator();

	mReadableControlFlowGenerator = new ReadableControlFlowGenerator(mRepo
			, mErrorReporter, *mCustomizer, *mValidator, mDiagram, this);
	mGotoControlFlowGenerator = new GotoControlFlowGenerator(mRepo
			, mErrorReporter, *mCustomizer, *mValidator, mDiagram, this);
}

QString MasterGeneratorBase::generate(const QString &indentString)
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
	const semantics::SemanticTree *mainControlFlow = mReadableControlFlowGenerator->generate();
	if (mainControlFlow && !mReadableControlFlowGenerator->cantBeGeneratedIntoStructuredCode()) {
		mainCode = mainControlFlow->toString(1, indentString);
		const bool subprogramsResult = mCustomizer->factory()->subprograms()->generate(mReadableControlFlowGenerator
				, indentString);
		if (!subprogramsResult) {
			if (supportsGotoGeneration()) {
				mainCode = QString();
			} else {
				return QString();
			}
		}
	} else {
		if (mReadableControlFlowGenerator->errorsOccured()) {
			return QString();
		}
	}

	if (mainCode.isEmpty() && supportsGotoGeneration()) {
		mErrorReporter.addInformation(tr("This diagram cannot be generated into the structured code."\
				" Generating it into the code with 'goto' statements."));
		const semantics::SemanticTree *gotoMainControlFlow = mGotoControlFlowGenerator->generate();
		if (gotoMainControlFlow) {
			mainCode = gotoMainControlFlow->toString(1, indentString);
			const bool gotoSubprogramsResult = mCustomizer->factory()
					->subprograms()->generate(mGotoControlFlowGenerator, indentString);
			if (!gotoSubprogramsResult) {
				mainCode = QString();
			}
		}
	}

	if (mainCode.isEmpty()) {
		const QString errorMessage = supportsGotoGeneration()
				? tr("This diagram cannot be even generated into the code with 'goto'"\
						"statements. Please contact the developers (WTF did you do?)")
				: tr("This diagram cannot be generated into the structured code.");
		mErrorReporter.addError(errorMessage);
		return QString();
	}

	QString resultCode = readTemplate("main.t");
	resultCode.replace("@@SUBPROGRAMS_FORWARDING@@", mCustomizer->factory()->subprograms()->forwardDeclarations());
	resultCode.replace("@@SUBPROGRAMS@@", mCustomizer->factory()->subprograms()->implementations());
	resultCode.replace("@@THREADS_FORWARDING@@", mCustomizer->factory()->threads().generateDeclarations());
	resultCode.replace("@@THREADS@@", mCustomizer->factory()->threads().generateImplementations(indentString));
	resultCode.replace("@@MAIN_CODE@@", mainCode);
	resultCode.replace("@@INITHOOKS@@", utils::StringUtils::addIndent(
			mCustomizer->factory()->initCode(), 1, indentString));
	resultCode.replace("@@TERMINATEHOOKS@@", utils::StringUtils::addIndent(
			mCustomizer->factory()->terminateCode(), 1, indentString));
	resultCode.replace("@@USERISRHOOKS@@", utils::StringUtils::addIndent(
			mCustomizer->factory()->isrHooksCode(), 1, indentString));
	resultCode.replace("@@VARIABLES@@", mCustomizer->factory()->variables()->generateVariableString());
	// This will remove too many empty lines
	resultCode.replace(QRegExp("\n(\n)+"), "\n\n");

	processGeneratedCode(resultCode);

	const QString pathToOutput = targetPath();
	outputCode(pathToOutput, resultCode);

	afterGeneration();

	return pathToOutput;
}

lua::LuaProcessor *MasterGeneratorBase::createLuaProcessor()
{
	return new lua::LuaProcessor(mErrorReporter, mTextLanguage, mParserErrorReporter, this);
}

PrimaryControlFlowValidator *MasterGeneratorBase::createValidator()
{
	return new PrimaryControlFlowValidator(mRepo, mErrorReporter, *mCustomizer, this);
}

void MasterGeneratorBase::beforeGeneration()
{
}

void MasterGeneratorBase::processGeneratedCode(QString &generatedCode)
{
	Q_UNUSED(generatedCode)
}

void MasterGeneratorBase::afterGeneration()
{
}

void MasterGeneratorBase::outputCode(const QString &path, const QString &code)
{
	QFile::remove(path);

	// File creation and modified timestamps should be equal after all.
	// But straitforward removal will not work due to the tunel file system effect:
	// http://stackoverflow.com/questions/661977/why-windows-sets-new-created-files-created-time-property-to-old-time
	utils::OutFile out(path);
	utils::FileSystemUtils::setCreationDateToNow(path);
	out() << code;
}
