#include "nxtOsekMasterGenerator.h"

#include <qrutils/stringUtils.h>
#include <generatorBase/parts/images.h>
#include <generatorBase/parts/threads.h>
#include "nxtOsekGeneratorCustomizer.h"

using namespace nxtOsek;

NxtOsekMasterGenerator::NxtOsekMasterGenerator(qrRepo::RepoApi const &repo
		, qReal::ErrorReporterInterface &errorReporter
		, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager
		, qrtext::LanguageToolboxInterface &textLanguage
		, qReal::Id const &diagramId)
	: MasterGeneratorBase(repo, errorReporter, robotModelManager, textLanguage, diagramId)
{
}

void NxtOsekMasterGenerator::generateOilAndMakeFiles()
{
	afterGeneration();
}

generatorBase::GeneratorCustomizer *NxtOsekMasterGenerator::createCustomizer()
{
	return new NxtOsekGeneratorCustomizer(mRepo, mErrorReporter, mRobotModelManager, *createLuaProcessor());
}

QString NxtOsekMasterGenerator::targetPath()
{
	return QString("%1/%2.c").arg(mProjectDir, mProjectName);
}

bool NxtOsekMasterGenerator::supportsGotoGeneration() const
{
	return true;
}

void NxtOsekMasterGenerator::afterGeneration()
{
	generateOilFile(mProjectName, mProjectDir);
	generateMakeFile(mProjectName, mProjectDir);
	saveImages(mProjectDir);
}

void NxtOsekMasterGenerator::generateOilFile(QString const &projectName
		, QString const &projectDir)
{
	QStringList tasks;
	for (QString const &task : mCustomizer->factory()->threads().threadNames()) {
		tasks << generateOilTask(task);
	}

	QString const resultOil = readTemplate("oil.t").replace("@@TASKS@@", tasks.join("\n"));
	outputCode(projectDir + "/" + projectName + ".oil", resultOil);
}

QString NxtOsekMasterGenerator::generateOilTask(QString const &taskName)
{
	return utils::StringUtils::addIndent(readTemplate("oilTask.t").replace("@@NAME@@", taskName), 1);
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
		images[fileName].save(projectDir + '/' + fileName + ".bmp", "BMP", -1);
	}
}
