#include "nxtGeneratorBase/nxtMasterGeneratorBase.h"

#include <qrutils/stringUtils.h>
#include <generatorBase/parts/images.h>
#include <generatorBase/parts/threads.h>
#include "nxtGeneratorCustomizer.h"

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

void NxtMasterGeneratorBase::afterGeneration()
{
	saveImages(mProjectDir);
}

void NxtMasterGeneratorBase::saveImages(QString const &projectDir)
{
	QMap<QString, QImage> &images = mCustomizer->factory()->images()->bmpFiles();
	foreach (QString const &fileName, images.keys()) {
		images[fileName].save(projectDir + '/' + fileName + ".bmp", "BMP", -1);
	}
}
