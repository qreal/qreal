#include "nxtOsekGeneratorCustomizer.h"

using namespace nxtOsek;

NxtOsekGeneratorCustomizer::NxtOsekGeneratorCustomizer(qrRepo::RepoApi const &repo
		, qReal::ErrorReporterInterface &errorReporter
		, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager)
	: mFactory(repo, errorReporter, robotModelManager)
{
}

generatorBase::GeneratorFactoryBase *NxtOsekGeneratorCustomizer::factory()
{
	return &mFactory;
}
