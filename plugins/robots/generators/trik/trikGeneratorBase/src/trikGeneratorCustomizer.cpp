#include "trikGeneratorCustomizer.h"

using namespace trik;

TrikGeneratorCustomizer::TrikGeneratorCustomizer(const qrRepo::RepoApi &repo
		, qReal::ErrorReporterInterface &errorReporter
		, const kitBase::robotModel::RobotModelManagerInterface &robotModelManager
		, generatorBase::lua::LuaProcessor &luaProcessor
		, const QString &generatorName)
	: mFactory(repo, errorReporter, robotModelManager, luaProcessor, generatorName)
{
}

generatorBase::GeneratorFactoryBase *TrikGeneratorCustomizer::factory()
{
	return &mFactory;
}
