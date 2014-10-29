#include "nxtGeneratorBase/nxtMasterGeneratorBase.h"

#include "nxtGeneratorCustomizer.h"
#include "nxtGeneratorBase/parts/images.h"

using namespace nxt;

NxtMasterGeneratorBase::NxtMasterGeneratorBase(qrRepo::RepoApi const &repo
		, qReal::ErrorReporterInterface &errorReporter
		, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager
		, qrtext::LanguageToolboxInterface &textLanguage
		, qReal::Id const &diagramId
		, QString const &generatorName)
	: MasterGeneratorBase(repo, errorReporter, robotModelManager, textLanguage, diagramId)
	, mGeneratorName(generatorName)
{
}

generatorBase::GeneratorCustomizer *NxtMasterGeneratorBase::createCustomizer()
{
	return new NxtGeneratorCustomizer(mRepo, mErrorReporter, mRobotModelManager, *createLuaProcessor(), mGeneratorName);
}

void NxtMasterGeneratorBase::beforeGeneration()
{
	static_cast<NxtGeneratorFactory *>(mCustomizer->factory())->images().reinit();
}

void NxtMasterGeneratorBase::processGeneratedCode(QString &code)
{
	code.replace("@@BMP_FILES@@", static_cast<NxtGeneratorFactory *>(mCustomizer->factory())->images().generate());
}

void NxtMasterGeneratorBase::afterGeneration()
{
	saveImages(mProjectDir);
}

void NxtMasterGeneratorBase::saveImages(QString const &projectDir)
{
	QMap<QString, QImage> &images = static_cast<NxtGeneratorFactory *>(mCustomizer->factory())->images().bmpFiles();
	for (QString const &fileName : images.keys()) {
		images[fileName].save(projectDir + '/' + fileName + ".bmp", "BMP", -1);
	}
}
