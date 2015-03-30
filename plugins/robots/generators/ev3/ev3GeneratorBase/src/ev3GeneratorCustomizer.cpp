#include "ev3GeneratorCustomizer.h"

using namespace ev3;

Ev3GeneratorCustomizer::Ev3GeneratorCustomizer(const qrRepo::RepoApi &repo
		, qReal::ErrorReporterInterface &errorReporter
		, const interpreterBase::robotModel::RobotModelManagerInterface &robotModelManager
		, generatorBase::lua::LuaProcessor &luaProcessor
		, const QString &generatorName)
	: mFactory(repo, errorReporter, robotModelManager, luaProcessor, generatorName)
{
}

generatorBase::GeneratorFactoryBase *Ev3GeneratorCustomizer::factory()
{
	return &mFactory;
}
