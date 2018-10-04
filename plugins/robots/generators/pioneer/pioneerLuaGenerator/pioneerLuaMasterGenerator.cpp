/* Copyright 2017 QReal Research Group
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

#include "pioneerLuaMasterGenerator.h"

#include <QtCore/QDir>

#include <qrkernel/logging.h>
#include <qrkernel/exception/exception.h>
#include <qrtext/languageToolboxInterface.h>
#include <qrutils/stringUtils.h>
#include <generatorBase/parts/initTerminateCodeGenerator.h>
#include <generatorBase/gotoControlFlowGenerator.h>
#include <generatorBase/parts/subprograms.h>
#include <generatorBase/parts/threads.h>
#include <generatorBase/parts/variables.h>

#include "pioneerLuaGeneratorCustomizer.h"
#include "pioneerLuaGeneratorFactory.h"
#include "generators/pioneerStateMachineGenerator.h"
#include "generators/gotoLabelManager.h"
#include "generators/randomFunctionChecker.h"

using namespace pioneer::lua;
using namespace generatorBase;

using namespace qReal;

PioneerLuaMasterGenerator::PioneerLuaMasterGenerator(const qrRepo::RepoApi &repo
		, qReal::ErrorReporterInterface &errorReporter
		, const utils::ParserErrorReporter &parserErrorReporter
		, const kitBase::robotModel::RobotModelManagerInterface &robotModelManager
		, qrtext::LanguageToolboxInterface &textLanguage
		, const qReal::Id &diagramId
		, const QString &generatorName
		, const qReal::EditorManagerInterface &metamodel)
	: MasterGeneratorBase(repo, errorReporter, robotModelManager, textLanguage, parserErrorReporter, diagramId)
	, mGeneratorName(generatorName)
	, mMetamodel(metamodel)
	, mGotoLabelManager(new GotoLabelManager())
{
}

PioneerLuaMasterGenerator::~PioneerLuaMasterGenerator()
{
	// Empty destructor to keep QScopedPointer happy.
}

void PioneerLuaMasterGenerator::initialize()
{
	MasterGeneratorBase::initialize();
	mControlFlowGenerator.reset(
			new PioneerStateMachineGenerator(mRepo, mErrorReporter, *mCustomizer, *mValidator, mDiagram)
			);

	auto factory = dynamic_cast<PioneerLuaGeneratorFactory *>(mCustomizer->factory());
	if (!factory) {
		throw qReal::Exception("PioneerLuaMasterGenerator shall work only with PioneerLuaGeneratorFactory");
	}

	mRandomFunctionChecker.reset(
			new RandomFunctionChecker(mRepo, mMetamodel, mTextLanguage, factory->randomGeneratorPart()));

	mControlFlowGenerator->registerNodeHook([this](const qReal::Id &id){ mRandomFunctionChecker->checkNode(id); });
}

generatorBase::GeneratorCustomizer *PioneerLuaMasterGenerator::createCustomizer()
{
	return new PioneerLuaGeneratorCustomizer(
			mRepo
			, mErrorReporter
			, mRobotModelManager
			, *createLuaProcessor()
			, mGeneratorName
			, *mGotoLabelManager
			, supportsSwitchUnstableToBreaks());
}

QString PioneerLuaMasterGenerator::targetPath()
{
	return QString("%1/%2.lua").arg(mProjectDir, mProjectName);
}

bool PioneerLuaMasterGenerator::supportsGotoGeneration() const
{
	return true;
}

void PioneerLuaMasterGenerator::beforeGeneration()
{
	QDir().mkpath(mProjectDir + "/ap/");
	QDir().mkpath(mProjectDir + "/Ev/");
	QFile::copy(":/pioneer/lua/templates/testStub/ap/lua.lua", mProjectDir + "/ap/lua.lua");
	QFile::copy(":/pioneer/lua/templates/testStub/Ev/lua.lua", mProjectDir + "/Ev/lua.lua");
}

QString PioneerLuaMasterGenerator::generate(const QString &indentString)
{
	if (mDiagram.isNull()) {
		mErrorReporter.addCritical(QObject::tr("There is no opened diagram"));
		return QString();
	}

	QLOG_INFO() << "Starting Pioneer program generation to " << mProjectDir;

	mGotoLabelManager->reinit();

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

	const semantics::SemanticTree *mainControlFlow = mControlFlowGenerator->generate();
	if (mainControlFlow) {
		mainCode = mainControlFlow->toString(0, indentString);
		const parts::Subprograms::GenerationResult subprogramsResult = mCustomizer->factory()
				->subprograms()->generate(mControlFlowGenerator.data(), indentString);
		if (subprogramsResult != parts::Subprograms::GenerationResult::success) {
			mainCode = QString();
		}
	}

	if (mainCode.isEmpty()) {
		const QString errorMessage = tr("Generation failed. Possible causes are internal error in generator or too "
				"complex program structure.");
		mErrorReporter.addError(errorMessage);
		return QString();
	}

	QString resultCode = readTemplate("main.t");
	replaceWithAutoIndent(resultCode, "@@SUBPROGRAMS_FORWARDING@@"
			, mCustomizer->factory()->subprograms()->forwardDeclarations());
	replaceWithAutoIndent(resultCode, "@@SUBPROGRAMS@@"
			, mCustomizer->factory()->subprograms()->implementations());
	replaceWithAutoIndent(resultCode, "@@THREADS_FORWARDING@@"
			, mCustomizer->factory()->threads().generateDeclarations());
	replaceWithAutoIndent(resultCode, "@@THREADS@@"
			, mCustomizer->factory()->threads().generateImplementations(indentString));
	replaceWithAutoIndent(resultCode, "@@MAIN_CODE@@", mainCode);
	replaceWithAutoIndent(resultCode, "@@INITHOOKS@@", mCustomizer->factory()->initCode());
	replaceWithAutoIndent(resultCode, "@@TERMINATEHOOKS@@", utils::StringUtils::addIndent(
			mCustomizer->factory()->terminateCode(), 1, indentString));
	replaceWithAutoIndent(resultCode, "@@USERISRHOOKS@@", utils::StringUtils::addIndent(
			mCustomizer->factory()->isrHooksCode(), 1, indentString));
	const QString constantsString = mCustomizer->factory()->variables()->generateConstantsString();
	const QString variablesString = mCustomizer->factory()->variables()->generateVariableString();

	if (resultCode.contains("@@CONSTANTS@@")) {
		replaceWithAutoIndent(resultCode, "@@CONSTANTS@@", constantsString);
		replaceWithAutoIndent(resultCode, "@@VARIABLES@@", variablesString);
	} else {
		replaceWithAutoIndent(resultCode, "@@VARIABLES@@", constantsString + "\n" + variablesString);
	}

	// This will remove too many empty lines
	resultCode.replace(QRegExp("\n(\n)+"), "\n\n");

	// This will remove leading and trailing whitespaces, line breaks and other unneeded stuff.
	resultCode = resultCode.trimmed();

	processGeneratedCode(resultCode);

	generateLinkingInfo(resultCode);

	const QString pathToOutput = targetPath();
	outputCode(pathToOutput, resultCode);

	afterGeneration();

	return pathToOutput;
}
