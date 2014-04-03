#include "russianCMasterGenerator.h"
#include "russianCGeneratorCustomizer.h"

using namespace qReal::robots::generators::russianC;

RussianCMasterGenerator::RussianCMasterGenerator(qrRepo::RepoApi const &repo
		, ErrorReporterInterface &errorReporter
		, Id const &diagramId)
	: MasterGeneratorBase(repo, errorReporter, diagramId)
{
}

qReal::robots::generators::GeneratorCustomizer *RussianCMasterGenerator::createCustomizer()
{
	return new RussianCGeneratorCustomizer(mRepo, mErrorReporter);
}

QString RussianCMasterGenerator::targetPath()
{
	return QString("%1/%2.c").arg(mProjectDir, mProjectName);
}

bool RussianCMasterGenerator::supportsGotoGeneration() const
{
	return false;
}

void RussianCMasterGenerator::afterGeneration()
{
	saveImages(mProjectDir);
}

void RussianCMasterGenerator::saveImages(QString const &projectDir)
{
	QMap<QString, QImage> &images = mCustomizer->factory()->images()->bmpFiles();
	foreach (QString const &fileName, images.keys()) {
		images[fileName].save(projectDir + '/' + fileName + ".bmp", "BMP", -1);
	}
}
