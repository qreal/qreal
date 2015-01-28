#include "nxtOsekCMasterGenerator.h"

#include <qrutils/stringUtils.h>
#include <generatorBase/parts/threads.h>
#include <nxtGeneratorBase/nxtGeneratorFactory.h>
#include <nxtGeneratorBase/parts/images.h>

using namespace nxt::osekC;

NxtOsekCMasterGenerator::NxtOsekCMasterGenerator(qrRepo::RepoApi const &repo
		, qReal::ErrorReporterInterface &errorReporter
		, const utils::ParserErrorReporter &parserErrorReporter
		, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager
		, qrtext::LanguageToolboxInterface &textLanguage
		, qReal::Id const &diagramId
		, QString const &generatorName)
	: NxtMasterGeneratorBase(repo, errorReporter, parserErrorReporter, robotModelManager, textLanguage, diagramId
			, generatorName)
{
}

void NxtOsekCMasterGenerator::generateOilAndMakeFiles()
{
	afterGeneration();
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
	NxtMasterGeneratorBase::afterGeneration();
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
	return utils::StringUtils::addIndent(readTemplate("oilTask.t").replace("@@NAME@@", taskName), 1, "\t");
}

void NxtOsekCMasterGenerator::generateMakeFile(QString const &projectName
		, QString const &projectDir)
{
	QString const bmps = static_cast<NxtGeneratorFactory *>(mCustomizer->factory())->images()
			.generateBmpFilesStringForMake().toUtf8();

	QString makefileTemplate = readTemplate("makefile.t");
	outputCode(projectDir + "/makefile", makefileTemplate
			.replace("@@PROJECT_NAME@@", projectName.toUtf8())
			.replace("@@BMPFILES@@", bmps));
}
