#include "ev3RbfMasterGenerator.h"

#include <qrtext/languageToolboxInterface.h>
#include <qrutils/stringUtils.h>

#include <generatorBase/parts/initTerminateCodeGenerator.h>
#include <generatorBase/gotoControlFlowGenerator.h>
#include <generatorBase/parts/subprograms.h>
#include <generatorBase/parts/threads.h>
#include <generatorBase/parts/variables.h>

using namespace ev3::rbf;
using namespace generatorBase;

using namespace qReal;

Ev3RbfMasterGenerator::Ev3RbfMasterGenerator(qrRepo::RepoApi const &repo
		, qReal::ErrorReporterInterface &errorReporter
		, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager
		, qrtext::LanguageToolboxInterface &textLanguage
		, qReal::Id const &diagramId
		, QString const &generatorName)
	: Ev3MasterGeneratorBase(repo, errorReporter, robotModelManager, textLanguage, diagramId, generatorName)
{
}

QString Ev3RbfMasterGenerator::generate()
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
	mErrorReporter.addInformation(tr("Generating code with 'goto' statements."));
	semantics::SemanticTree const *gotoMainControlFlow = mGotoControlFlowGenerator->generate();
	if (gotoMainControlFlow) {
		mainCode = gotoMainControlFlow->toString(1);
		bool const gotoSubprogramsResult = mCustomizer->factory()
				->subprograms()->generate(mGotoControlFlowGenerator);
		if (!gotoSubprogramsResult) {
			mainCode = QString();
		}
	}

	if (mainCode.isEmpty()) {
		mErrorReporter.addError(tr("This diagram cannot be even generated into the code with 'goto'"\
								"statements. Please contact the developers (WTF did you do?)"));
		return QString();
	}

	QString resultCode = readTemplate("main.t");
	resultCode.replace("@@SUBPROGRAMS_FORWARDING@@", mCustomizer->factory()->subprograms()->forwardDeclarations());
	resultCode.replace("@@SUBPROGRAMS@@", mCustomizer->factory()->subprograms()->implementations());
	resultCode.replace("@@THREADS_FORWARDING@@", mCustomizer->factory()->threads().generateDeclarations());
	resultCode.replace("@@THREADS@@", mCustomizer->factory()->threads().generateImplementations());
	resultCode.replace("@@MAIN_CODE@@", mainCode);
	resultCode.replace("@@INITHOOKS@@", utils::StringUtils::addIndent(
			mCustomizer->factory()->initCode(), 1));
	resultCode.replace("@@TERMINATEHOOKS@@", utils::StringUtils::addIndent(
			mCustomizer->factory()->terminateCode(), 1));
	resultCode.replace("@@USERISRHOOKS@@", utils::StringUtils::addIndent(
			mCustomizer->factory()->isrHooksCode(), 1));
	resultCode.replace("@@VARIABLES@@", mCustomizer->factory()->variables()->generateVariableString());
	// This will remove too many empty lines
	resultCode.replace(QRegExp("\n(\n)+"), "\n\n");

	processGeneratedCode(resultCode);

	QString const pathToOutput = targetPath();
	outputCode(pathToOutput, resultCode);

	afterGeneration();

	return pathToOutput;
}

QString Ev3RbfMasterGenerator::targetPath()
{
	return QString("%1/%2.rbf").arg(mProjectDir, mProjectName);
}

bool Ev3RbfMasterGenerator::supportsGotoGeneration() const
{
	return true;
}
