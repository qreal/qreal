#include "nxtOsekCMasterGenerator.h"

#include <qrutils/stringUtils.h>
#include <generatorBase/parts/threads.h>
#include <nxtGeneratorBase/nxtGeneratorFactory.h>
#include <nxtGeneratorBase/parts/images.h>

using namespace nxt::osekC;

NxtOsekCMasterGenerator::NxtOsekCMasterGenerator(const qrRepo::RepoApi &repo
		, qReal::ErrorReporterInterface &errorReporter
		, const utils::ParserErrorReporter &parserErrorReporter
		, const kitBase::robotModel::RobotModelManagerInterface &robotModelManager
		, qrtext::LanguageToolboxInterface &textLanguage
		, const qReal::Id &diagramId
		, const QString &generatorName)
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

void NxtOsekCMasterGenerator::generateOilFile(const QString &projectName
		, const QString &projectDir)
{
	QStringList tasks;
	for (const QString &task : mCustomizer->factory()->threads().threadNames()) {
		tasks << generateOilTask(task);
	}

	const QString resultOil = readTemplate("oil.t").replace("@@TASKS@@", tasks.join("\n"));
	outputCode(projectDir + "/" + projectName + ".oil", resultOil);
}

QString NxtOsekCMasterGenerator::generateOilTask(const QString &taskName)
{
	return utils::StringUtils::addIndent(readTemplate("oilTask.t").replace("@@NAME@@", taskName), 1, "\t");
}

void NxtOsekCMasterGenerator::generateMakeFile(const QString &projectName
		, const QString &projectDir)
{
	const QString bmps = static_cast<NxtGeneratorFactory *>(mCustomizer->factory())->images()
			.generateBmpFilesStringForMake().toUtf8();

	QString makefileTemplate = readTemplate("makefile.t");
	outputCode(projectDir + "/makefile", makefileTemplate
			.replace("@@PROJECT_NAME@@", projectName.toUtf8())
			.replace("@@BMPFILES@@", bmps));
}
