#include "nxtOsekMasterGenerator.h"
#include "nxtOsekGeneratorCustomizer.h"

using namespace qReal::robots::generators::nxtOsek;

NxtOsekMasterGenerator::NxtOsekMasterGenerator(qrRepo::RepoApi const &repo
		, ErrorReporterInterface &errorReporter
		, Id const &diagramId)
	: MasterGeneratorBase(repo, errorReporter, diagramId)
{
}

qReal::robots::generators::GeneratorCustomizer *NxtOsekMasterGenerator::createCustomizer()
{
	return new NxtOsekGeneratorCustomizer(mRepo, mErrorReporter);
}

QString NxtOsekMasterGenerator::pathToGenerate()
{
	return QString("%1/%2.c").arg(mProjectDir, mProjectName);
}

void NxtOsekMasterGenerator::beforeGeneration()
{
	// TODO: increment it or smth...
	mCurInitialNodeNumber = 0;
	mProjectName = "example" + QString::number(mCurInitialNodeNumber);
	mProjectDir = "nxt-tools/" + mProjectName;
	createProjectDir(mProjectDir);
}

void NxtOsekMasterGenerator::afterGeneration()
{
	generateOilFile(mProjectName, mProjectDir);
	generateMakeFile(mProjectName, mProjectDir);
	saveImages(mProjectDir);
}

void NxtOsekMasterGenerator::createProjectDir(QString const &projectDir)
{
	if (!QDir(projectDir).exists()) {
		if (!QDir("nxt-tools/").exists()) {
			QDir().mkdir("nxt-tools/");
		}
		QDir().mkdir(projectDir);
	}
}

void NxtOsekMasterGenerator::generateOilFile(QString const &projectName
		, QString const &projectDir)
{
	QString const taskNumber = QString::number(mCurInitialNodeNumber);
	QString const task = readTemplate("oilTask.t").replace("@@NUMBER@@", taskNumber);
	QString const resultOil = readTemplate("oil.t").replace("@@TASK@@", task);
	outputCode(projectDir + "/" + projectName + ".oil", resultOil);
}

void NxtOsekMasterGenerator::generateMakeFile(QString const &projectName
		, QString const &projectDir)
{
	QString const bmps = mCustomizer->factory()->images()->generateBmpFilesStringForMake().toUtf8();

	QString makefileTemplate = readTemplate("makefile.t");
	outputCode(projectDir + "/makefile", makefileTemplate
			.replace("@@PROJECT_NAME@@", projectName.toUtf8())
			.replace("@@BMPFILES@@", bmps));
}

void NxtOsekMasterGenerator::saveImages(QString const &projectDir)
{
	QMap<QString, QImage> &images = mCustomizer->factory()->images()->bmpFiles();
	foreach (QString const &fileName, images.keys()) {
		images[fileName].save(projectDir + fileName + ".bmp", "BMP", -1);
	}
}
