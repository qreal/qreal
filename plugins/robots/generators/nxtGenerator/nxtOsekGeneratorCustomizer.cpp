#include "nxtOsekGeneratorCustomizer.h"

using namespace nxtOsek;

NxtOsekGeneratorCustomizer::NxtOsekGeneratorCustomizer(qrRepo::RepoApi const &repo
		, qReal::ErrorReporterInterface &errorReporter
		, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager
		, generatorBase::lua::LuaProcessor &luaProcessor)
	: mFactory(repo, errorReporter, robotModelManager, luaProcessor)
{
}

generatorBase::GeneratorFactoryBase *NxtOsekGeneratorCustomizer::factory()
{
	return &mFactory;
}
