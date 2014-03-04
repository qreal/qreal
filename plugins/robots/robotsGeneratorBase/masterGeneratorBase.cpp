#include "masterGeneratorBase.h"

#include <qrutils/outFile.h>
#include <qrutils/stringUtils.h>
#include <readableControlFlowGenerator.h>
#include <gotoControlFlowGenerator.h>

using namespace qReal::robots::generators;

MasterGeneratorBase::MasterGeneratorBase(qrRepo::RepoApi const &repo
		, ErrorReporterInterface &errorReporter
		, Id const &diagramId)
	: mRepo(repo)
	, mErrorReporter(errorReporter)
	, mDiagram(diagramId)
{
}

void MasterGeneratorBase::setProjectDir(QFileInfo const &fileInfo)
{
	mProjectName = fileInfo.baseName();
	mProjectDir = fileInfo.absolutePath();
}

void MasterGeneratorBase::initialize()
{
	mCustomizer = createCustomizer();
	mCustomizer->factory()->initialize();
	setPathToTemplates(mCustomizer->factory()->pathToTemplates());

//	mReadableControlFlowGenerator = new ReadableControlFlowGenerator(mRepo
//			, mErrorReporter, *mCustomizer, mDiagram, this);
	mReadableControlFlowGenerator = new GotoControlFlowGenerator(mRepo
			, mErrorReporter, *mCustomizer, mDiagram, this);
}

QString MasterGeneratorBase::generate()
{
	if (mDiagram.isNull()) {
		mErrorReporter.addCritical(QObject::tr("There is no opened diagram"));
		return QString();
	}

	beforeGeneration();
	if (!QDir(mProjectDir).exists()) {
		QDir().mkpath(mProjectDir);
	}

	mCustomizer->factory()->variables()->reinit(mRepo);

	mCustomizer->factory()->images()->reinit();

	foreach (parts::InitTerminateCodeGenerator *generator, mCustomizer->factory()->initTerminateGenerators()) {
		generator->reinit();
	}

	semantics::SemanticTree const *mainControlFlow = mReadableControlFlowGenerator->generate();
	if (!mainControlFlow) {
		return QString();
	}

	QString const mainCode = mainControlFlow->toString(1);
	bool const subprogramsResult = mCustomizer->factory()->subprograms()->generate(mReadableControlFlowGenerator);
	if (!subprogramsResult) {
		return QString();
	}

	QString resultCode = readTemplate("main.t");
	resultCode.replace("@@SUBPROGRAMS@@", mCustomizer->factory()->subprograms()->generatedCode());
	resultCode.replace("@@MAIN_CODE@@", mainCode);
	resultCode.replace("@@INITHOOKS@@", utils::StringUtils::addIndent(
			mCustomizer->factory()->initCode(), 1));
	resultCode.replace("@@TERMINATEHOOKS@@", utils::StringUtils::addIndent(
			mCustomizer->factory()->terminateCode(), 1));
	resultCode.replace("@@USERISRHOOKS@@", utils::StringUtils::addIndent(
			mCustomizer->factory()->isrHooksCode(), 1));
	resultCode.replace("@@BMP_FILES@@", mCustomizer->factory()->images()->generate());
	resultCode.replace("@@VARIABLES@@", mCustomizer->factory()->variables()->generateVariableString());

	QString const pathToOutput = targetPath();
	outputCode(pathToOutput, resultCode);

	afterGeneration();

	return pathToOutput;
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

void MasterGeneratorBase::outputCode(QString const &path, QString const &code)
{
	utils::OutFile out(path);
	out() << code;
}
