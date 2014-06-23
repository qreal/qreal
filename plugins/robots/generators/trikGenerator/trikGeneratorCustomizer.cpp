#include "trikGeneratorCustomizer.h"

using namespace trik;

TrikGeneratorCustomizer::TrikGeneratorCustomizer(qrRepo::RepoApi const &repo
		, qReal::ErrorReporterInterface &errorReporter
		, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager)
	: mFactory(repo, errorReporter, robotModelManager)
{
}

generatorBase::GeneratorFactoryBase *TrikGeneratorCustomizer::factory()
{
	return &mFactory;
}
