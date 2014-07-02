#include "fSharpMasterGenerator.h"
#include "fSharpGeneratorCustomizer.h"


#include <qrutils/outFile.h>
#include <qrutils/stringUtils.h>


using namespace fSharp;

FSharpMasterGenerator::FSharpMasterGenerator(qrRepo::RepoApi const &repo
		, qReal::ErrorReporterInterface &errorReporter
		, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager
		, qReal::Id const &diagramId)
	: MasterGeneratorBase(repo, errorReporter, robotModelManager, diagramId)
{
}

generatorBase::GeneratorCustomizer *FSharpMasterGenerator::createCustomizer()
{
	return new FSharpGeneratorCustomizer(mRepo, mErrorReporter, mRobotModelManager);
}

QString FSharpMasterGenerator::targetPath()
{
	return QString("%1/%2.fsx").arg(mProjectDir, mProjectName);
}

bool FSharpMasterGenerator::supportsGotoGeneration() const
{
	return false;
}

void FSharpMasterGenerator::outputCode(const QString &path, const QString &code)
{
	utils::OutFile out(path);
	QString copy = code;
	out() << copy.replace('\t', "    ");
}
