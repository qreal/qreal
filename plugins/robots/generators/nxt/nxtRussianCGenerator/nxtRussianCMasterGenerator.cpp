#include "nxtRussianCMasterGenerator.h"
#include "nxtRussianCGeneratorCustomizer.h"

#include <generatorBase/parts/images.h>

using namespace russianC;
using namespace generatorBase;
using namespace qReal;

NxtRussianCMasterGenerator::NxtRussianCMasterGenerator(qrRepo::RepoApi const &repo
		, ErrorReporterInterface &errorReporter
		, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager
		, qrtext::LanguageToolboxInterface &textLanguage
		, Id const &diagramId)
	: MasterGeneratorBase(repo, errorReporter, robotModelManager, textLanguage, diagramId)
{
}

generatorBase::GeneratorCustomizer *NxtRussianCMasterGenerator::createCustomizer()
{
	return new NxtRussianCGeneratorCustomizer(mRepo, mErrorReporter, mRobotModelManager, *createLuaProcessor());
}

QString NxtRussianCMasterGenerator::targetPath()
{
	return QString::fromUtf8("%1/%2.си").arg(mProjectDir, mProjectName);
}

bool NxtRussianCMasterGenerator::supportsGotoGeneration() const
{
	return false;
}

void NxtRussianCMasterGenerator::afterGeneration()
{
	saveImages(mProjectDir);
}

void NxtRussianCMasterGenerator::saveImages(QString const &projectDir)
{
	QMap<QString, QImage> &images = mCustomizer->factory()->images()->bmpFiles();
	foreach (QString const &fileName, images.keys()) {
		images[fileName].save(projectDir + '/' + fileName + ".bmp", "BMP", -1);
	}
}
