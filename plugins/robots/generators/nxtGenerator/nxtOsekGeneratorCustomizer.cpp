#include "nxtOsekGeneratorCustomizer.h"

using namespace nxtOsek;

NxtOsekGeneratorCustomizer::NxtOsekGeneratorCustomizer(qrRepo::RepoApi const &repo
		, qReal::ErrorReporterInterface &errorReporter
		, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager
		, qrtext::LanguageToolboxInterface &textLanguage)
	: mFactory(repo, errorReporter, robotModelManager, textLanguage)
{
}

generatorBase::GeneratorFactoryBase *NxtOsekGeneratorCustomizer::factory()
{
	return &mFactory;
}
