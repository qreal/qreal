#include "fSharpMasterGenerator.h"
#include "fSharpGeneratorCustomizer.h"

#include <qrutils/outFile.h>
#include <qrutils/stringUtils.h>


using namespace qReal::robots::generators::fSharp;

FSharpMasterGenerator::FSharpMasterGenerator(qrRepo::RepoApi const &repo
		, ErrorReporterInterface &errorReporter
		, Id const &diagramId)
	: MasterGeneratorBase(repo, errorReporter, diagramId)
{
}

qReal::robots::generators::GeneratorCustomizer *FSharpMasterGenerator::createCustomizer()
{
    return new FSharpGeneratorCustomizer(mRepo, mErrorReporter);
}

QString FSharpMasterGenerator::targetPath()
{
	return QString("%1/%2.qts").arg(mProjectDir, mProjectName);
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
