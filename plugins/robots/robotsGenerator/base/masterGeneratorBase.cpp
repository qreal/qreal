#include "masterGeneratorBase.h"

#include <qrutils/outFile.h>
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

	semantics::SemanticTree const *mainControlFlow = mReadableControlFlowGenerator->generate();
	bool const subprogramsResult = mCustomizer->factory()->subprograms()->generate(mReadableControlFlowGenerator);
	if (!mainControlFlow || !subprogramsResult) {
		return QString();
	}

	QString resultCode = readTemplate("main.t");
	resultCode.replace("@@SUBPROGRAMS@@", mCustomizer->factory()->subprograms()->generatedCode());
	resultCode.replace("@@MAIN_CODE@@", mainControlFlow->toString(1));

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
