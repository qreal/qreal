#include "fSharpGeneratorCustomizer.h"

using namespace fSharp;

FSharpGeneratorCustomizer::FSharpGeneratorCustomizer(qrRepo::RepoApi const &repo
		, qReal::ErrorReporterInterface &errorReporter
		, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager)
	: mFactory(repo, errorReporter, robotModelManager)
{
}

generatorBase::GeneratorFactoryBase *FSharpGeneratorCustomizer::factory()
{
	return &mFactory;
}
