#include "russianCGeneratorCustomizer.h"

using namespace russianC;

RussianCGeneratorCustomizer::RussianCGeneratorCustomizer(qrRepo::RepoApi const &repo
		, qReal::ErrorReporterInterface &errorReporter
		, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager)
	: mFactory(new RussianCGeneratorFactory(repo, errorReporter, robotModelManager))
{
}

generatorBase::GeneratorFactoryBase *RussianCGeneratorCustomizer::factory()
{
	return mFactory;
}
