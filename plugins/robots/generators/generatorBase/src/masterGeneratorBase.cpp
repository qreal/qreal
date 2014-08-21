#include "generatorBase/masterGeneratorBase.h"

#include <qrutils/outFile.h>
#include <qrutils/stringUtils.h>
#include "readableControlFlowGenerator.h"
#include "gotoControlFlowGenerator.h"

#include "generatorBase/parts/variables.h"
#include "generatorBase/parts/images.h"
#include "generatorBase/parts/subprograms.h"
#include "generatorBase/parts/sensors.h"
#include "generatorBase/parts/initTerminateCodeGenerator.h"

using namespace generatorBase;
using namespace qReal;

MasterGeneratorBase::MasterGeneratorBase(qrRepo::RepoApi const &repo
		, ErrorReporterInterface &errorReporter
		, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager
		, Id const &diagramId)
	: mRepo(repo)
	, mErrorReporter(errorReporter)
	, mRobotModelManager(robotModelManager)
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

	mReadableControlFlowGenerator = new ReadableControlFlowGenerator(mRepo
			, mErrorReporter, *mCustomizer, mDiagram, this);
	mGotoControlFlowGenerator = new GotoControlFlowGenerator(mRepo
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

	mCustomizer->factory()->setMainDiagramId(mDiagram);
	mCustomizer->factory()->variables()->reinit(mRepo);
	mCustomizer->factory()->images()->reinit();

	for (parts::InitTerminateCodeGenerator *generator : mCustomizer->factory()->initTerminateGenerators()) {
		generator->reinit();
	}

	QString mainCode;
	semantics::SemanticTree const *mainControlFlow = mReadableControlFlowGenerator->generate();
	if (mainControlFlow && !mReadableControlFlowGenerator->cantBeGeneratedIntoStructuredCode()) {
		mainCode = mainControlFlow->toString(1);
		bool const subprogramsResult = mCustomizer->factory()->subprograms()->generate(mReadableControlFlowGenerator);
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
		semantics::SemanticTree const *gotoMainControlFlow = mGotoControlFlowGenerator->generate();
		if (gotoMainControlFlow) {
			mainCode = gotoMainControlFlow->toString(1);
			bool const gotoSubprogramsResult = mCustomizer->factory()
					->subprograms()->generate(mGotoControlFlowGenerator);
			if (!gotoSubprogramsResult) {
				mainCode = QString();
			}
		}
	}

	if (mainCode.isEmpty()) {
		QString const errorMessage = supportsGotoGeneration()
				? tr("This diagram cannot be even generated into the code with 'goto'"\
						"statements. Please contact the developers (WTF did you do?)")
				: tr("This diagram cannot be generated into the structured code.");
		mErrorReporter.addError(errorMessage);
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
	// File must be removed to leave created and modified timestamps equal.
	QFile::remove(path);
	utils::OutFile out(path);
	out() << code;
}
