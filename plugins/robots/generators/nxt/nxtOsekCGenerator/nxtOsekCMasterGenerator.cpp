#include "nxtOsekCMasterGenerator.h"

#include <qrutils/stringUtils.h>
#include <generatorBase/parts/images.h>
#include <generatorBase/parts/threads.h>
#include "nxtOsekCGeneratorCustomizer.h"

using namespace nxtOsek;

NxtOsekCMasterGenerator::NxtOsekCMasterGenerator(qrRepo::RepoApi const &repo
		, qReal::ErrorReporterInterface &errorReporter
		, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager
		, qrtext::LanguageToolboxInterface &textLanguage
		, qReal::Id const &diagramId)
	: MasterGeneratorBase(repo, errorReporter, robotModelManager, textLanguage, diagramId)
{
}

void NxtOsekCMasterGenerator::generateOilAndMakeFiles()
{
	afterGeneration();
}

generatorBase::GeneratorCustomizer *NxtOsekCMasterGenerator::createCustomizer()
{
	return new NxtOsekCGeneratorCustomizer(mRepo, mErrorReporter, mRobotModelManager, *createLuaProcessor());
}

QString NxtOsekCMasterGenerator::targetPath()
{
	return QString("%1/%2.c").arg(mProjectDir, mProjectName);
}

bool NxtOsekCMasterGenerator::supportsGotoGeneration() const
{
	return true;
}

void NxtOsekCMasterGenerator::afterGeneration()
{
	generateOilFile(mProjectName, mProjectDir);
	generateMakeFile(mProjectName, mProjectDir);
	saveImages(mProjectDir);
}

void NxtOsekCMasterGenerator::generateOilFile(QString const &projectName
		, QString const &projectDir)
{
	QStringList tasks;
	for (QString const &task : mCustomizer->factory()->threads().threadNames()) {
		tasks << generateOilTask(task);
	}

	QString const resultOil = readTemplate("oil.t").replace("@@TASKS@@", tasks.join("\n"));
	outputCode(projectDir + "/" + projectName + ".oil", resultOil);
}

QString NxtOsekCMasterGenerator::generateOilTask(QString const &taskName)
{
	return utils::StringUtils::addIndent(readTemplate("oilTask.t").replace("@@NAME@@", taskName), 1);
}

void NxtOsekCMasterGenerator::generateMakeFile(QString const &projectName
		, QString const &projectDir)
{
	QString const bmps = mCustomizer->factory()->images()->generateBmpFilesStringForMake().toUtf8();

	QString makefileTemplate = readTemplate("makefile.t");
	outputCode(projectDir + "/makefile", makefileTemplate
			.replace("@@PROJECT_NAME@@", projectName.toUtf8())
			.replace("@@BMPFILES@@", bmps));
}

void NxtOsekCMasterGenerator::saveImages(QString const &projectDir)
{
	QMap<QString, QImage> &images = mCustomizer->factory()->images()->bmpFiles();
	foreach (QString const &fileName, images.keys()) {
		images[fileName].save(projectDir + '/' + fileName + ".bmp", "BMP", -1);
	}
}
