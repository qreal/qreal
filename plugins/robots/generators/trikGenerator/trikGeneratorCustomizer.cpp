#include "trikGeneratorCustomizer.h"

using namespace trik;

TrikGeneratorCustomizer::TrikGeneratorCustomizer(qrRepo::RepoApi const &repo
		, qReal::ErrorReporterInterface &errorReporter
		, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager
		, qrtext::LanguageToolboxInterface &textLanguage)
	: mFactory(repo, errorReporter, robotModelManager, textLanguage)
{
}

generatorBase::GeneratorFactoryBase *TrikGeneratorCustomizer::factory()
{
	return &mFactory;
}
