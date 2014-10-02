#include "russianCGeneratorCustomizer.h"

using namespace russianC;

RussianCGeneratorCustomizer::RussianCGeneratorCustomizer(qrRepo::RepoApi const &repo
		, qReal::ErrorReporterInterface &errorReporter
		, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager
		, generatorBase::lua::LuaProcessor &luaProcessor)
	: mFactory(new RussianCGeneratorFactory(repo, errorReporter, robotModelManager, luaProcessor))
{
}

generatorBase::GeneratorFactoryBase *RussianCGeneratorCustomizer::factory()
{
	return mFactory;
}
