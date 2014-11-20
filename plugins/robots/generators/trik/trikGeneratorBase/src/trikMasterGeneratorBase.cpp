#include "trikGeneratorBase/trikMasterGeneratorBase.h"
#include "trikGeneratorCustomizer.h"

using namespace trik;

TrikMasterGeneratorBase::TrikMasterGeneratorBase(qrRepo::RepoApi const &repo
		, qReal::ErrorReporterInterface &errorReporter
		, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager
		, qrtext::LanguageToolboxInterface &textLanguage
		, qReal::Id const &diagramId
		, QString const &generatorName)
	: MasterGeneratorBase(repo, errorReporter, robotModelManager, textLanguage, diagramId)
	, mGeneratorName(generatorName)
{
}

generatorBase::GeneratorCustomizer *TrikMasterGeneratorBase::createCustomizer()
{
	return new TrikGeneratorCustomizer(mRepo, mErrorReporter
			, mRobotModelManager, *createLuaProcessor(), mGeneratorName);
}
