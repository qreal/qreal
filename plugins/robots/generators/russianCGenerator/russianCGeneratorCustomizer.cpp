#include "russianCGeneratorCustomizer.h"

using namespace russianC;

RussianCGeneratorCustomizer::RussianCGeneratorCustomizer(qrRepo::RepoApi const &repo
		, qReal::ErrorReporterInterface &errorReporter
		, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager
		, qrtext::LanguageToolboxInterface &textLanguage)
	: mFactory(new RussianCGeneratorFactory(repo, errorReporter, robotModelManager, textLanguage))
{
}

generatorBase::GeneratorFactoryBase *RussianCGeneratorCustomizer::factory()
{
	return mFactory;
}
