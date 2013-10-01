#include "trikMasterGenerator.h"
#include "trikGeneratorCustomizer.h"

using namespace qReal::robots::generators::trik;

QString const scriptExtension = ".qts";

TrikMasterGenerator::TrikMasterGenerator(qrRepo::RepoApi const &repo
		, ErrorReporterInterface &errorReporter
		, Id const &diagramId)
	: MasterGeneratorBase(repo, errorReporter, diagramId)
{
}

qReal::robots::generators::GeneratorCustomizer *TrikMasterGenerator::createCustomizer()
{
	return new TrikGeneratorCustomizer(mRepo, mErrorReporter);
}

QString TrikMasterGenerator::targetPath()
{
	return QString("%1/%2").arg(mProjectDir, mProjectName);
}

void TrikMasterGenerator::beforeGeneration()
{
//	mProjectName = currentProgramName();
//	mProjectDir = "trik/" + mProjectName;
//	createProjectDir(mProjectDir);
	mProjectDir = ".";
	mProjectName = currentProgramName();
}


void TrikMasterGenerator::createProjectDir(QString const &projectDir)
{
	Q_UNUSED(projectDir)
//	if (!QDir(projectDir).exists()) {
//		if (!QDir("trik/").exists()) {
//			QDir().mkdir("trik/");
//		}
//		QDir().mkdir(projectDir);
//	}
}

QString TrikMasterGenerator::currentProgramName() const
{
	QString const saveFileName = mRepo.workingFile();
	QFileInfo const fileInfo(saveFileName);
	return fileInfo.baseName() + scriptExtension;
}
