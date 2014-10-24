#include "nxtRussianCGeneratorCustomizer.h"

using namespace russianC;

NxtRussianCGeneratorCustomizer::NxtRussianCGeneratorCustomizer(qrRepo::RepoApi const &repo
		, qReal::ErrorReporterInterface &errorReporter
		, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager
		, generatorBase::lua::LuaProcessor &luaProcessor)
	: mFactory(new NxtRussianCGeneratorFactory(repo, errorReporter, robotModelManager, luaProcessor))
{
}

generatorBase::GeneratorFactoryBase *NxtRussianCGeneratorCustomizer::factory()
{
	return mFactory;
}
