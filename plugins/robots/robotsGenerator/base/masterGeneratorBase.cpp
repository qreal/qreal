#include "masterGeneratorBase.h"

#include <qrutils/outFile.h>
#include <qrutils/stringUtils.h>
#include "readableControlFlowGenerator.h"

using namespace qReal::robots::generators;

MasterGeneratorBase::MasterGeneratorBase(qrRepo::RepoApi const &repo
		, ErrorReporterInterface &errorReporter
		, Id const &diagramId)
	: mRepo(repo)
	, mErrorReporter(errorReporter)
	, mDiagram(diagramId)
{
}

void MasterGeneratorBase::initialize()
{
	mCustomizer = createCustomizer();
	mCustomizer->factory()->initialize();
	setPathToTemplates(mCustomizer->factory()->pathToTemplates());
	mReadableControlFlowGenerator = new ReadableControlFlowGenerator(mRepo
			, mErrorReporter, *mCustomizer, mDiagram, this);
}

QString MasterGeneratorBase::generate()
{
	if (mDiagram.isNull()) {
		mErrorReporter.addCritical(QObject::tr("There is no opened diagram"));
		return QString();
	}

	beforeGeneration();

	mCustomizer->factory()->variables()->reinit(mRepo);
	mCustomizer->factory()->images()->reinit();
	foreach (parts::InitTerminateCodeGenerator *generator, mCustomizer->factory()->initTerminateGenerators()) {
		generator->reinit();
	}

	semantics::SemanticTree const *mainControlFlow = mReadableControlFlowGenerator->generate();
	bool const subprogramsResult = mCustomizer->factory()->subprograms()->generate(mReadableControlFlowGenerator);
	if (!mainControlFlow || !subprogramsResult) {
		return QString();
	}

	QString const resultCode = readTemplate("main.t")
			.replace("@@SUBPROGRAMS@@", mCustomizer->factory()->subprograms()->generatedCode())
			.replace("@@MAIN_CODE@@", mainControlFlow->toString(1))
			.replace("@@INITHOOKS@@", utils::StringUtils::addIndent(
					mCustomizer->factory()->initCode(), 1))
			.replace("@@TERMINATEHOOKS@@", utils::StringUtils::addIndent(
					mCustomizer->factory()->terminateCode(), 1))
			.replace("@@USERISRHOOKS@@", utils::StringUtils::addIndent(
					mCustomizer->factory()->isrHooksCode(), 1))
			.replace("@@BMP_FILES@@", mCustomizer->factory()->images()->generate())
			.replace("@@VARIABLES@@", mCustomizer->factory()->variables()->generateVariableString());

	QString const pathToOutput = pathToGenerate();
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
