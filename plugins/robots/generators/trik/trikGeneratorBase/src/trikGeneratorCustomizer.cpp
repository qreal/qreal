#include "trikGeneratorCustomizer.h"

using namespace trik;

TrikGeneratorCustomizer::TrikGeneratorCustomizer(qrRepo::RepoApi const &repo
		, qReal::ErrorReporterInterface &errorReporter
		, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager
		, generatorBase::lua::LuaProcessor &luaProcessor
		, QString const &generatorName)
	: mFactory(repo, errorReporter, robotModelManager, luaProcessor, generatorName)
{
}

generatorBase::GeneratorFactoryBase *TrikGeneratorCustomizer::factory()
{
	return &mFactory;
}
