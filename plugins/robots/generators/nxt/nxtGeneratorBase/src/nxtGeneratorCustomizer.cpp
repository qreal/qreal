#include "nxtGeneratorCustomizer.h"

using namespace nxt;

NxtGeneratorCustomizer::NxtGeneratorCustomizer(const qrRepo::RepoApi &repo
		, qReal::ErrorReporterInterface &errorReporter
		, const kitBase::robotModel::RobotModelManagerInterface &robotModelManager
		, generatorBase::lua::LuaProcessor &luaProcessor
		, const QString &generatorName)
	: mFactory(repo, errorReporter, robotModelManager, luaProcessor, generatorName)
{
}

generatorBase::GeneratorFactoryBase *NxtGeneratorCustomizer::factory()
{
	return &mFactory;
}
