#include "nxtOsekCGeneratorCustomizer.h"

using namespace nxtOsek;

NxtOsekCGeneratorCustomizer::NxtOsekCGeneratorCustomizer(qrRepo::RepoApi const &repo
		, qReal::ErrorReporterInterface &errorReporter
		, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager
		, generatorBase::lua::LuaProcessor &luaProcessor)
	: mFactory(repo, errorReporter, robotModelManager, luaProcessor)
{
}

generatorBase::GeneratorFactoryBase *NxtOsekCGeneratorCustomizer::factory()
{
	return &mFactory;
}
