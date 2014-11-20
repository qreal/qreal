#include "nxtGeneratorCustomizer.h"

using namespace nxt;

NxtGeneratorCustomizer::NxtGeneratorCustomizer(qrRepo::RepoApi const &repo
		, qReal::ErrorReporterInterface &errorReporter
		, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager
		, generatorBase::lua::LuaProcessor &luaProcessor
		, QString const &generatorName)
	: mFactory(repo, errorReporter, robotModelManager, luaProcessor, generatorName)
{
}

generatorBase::GeneratorFactoryBase *NxtGeneratorCustomizer::factory()
{
	return &mFactory;
}
