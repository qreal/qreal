#include "trikQtsGeneratorCustomizer.h"

using namespace trik;

TrikQtsGeneratorCustomizer::TrikQtsGeneratorCustomizer(qrRepo::RepoApi const &repo
		, qReal::ErrorReporterInterface &errorReporter
		, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager
		, generatorBase::lua::LuaProcessor &luaProcessor)
	: mFactory(repo, errorReporter, robotModelManager, luaProcessor)
{
}

generatorBase::GeneratorFactoryBase *TrikQtsGeneratorCustomizer::factory()
{
	return &mFactory;
}
