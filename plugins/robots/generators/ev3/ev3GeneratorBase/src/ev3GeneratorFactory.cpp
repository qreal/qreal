#include "ev3GeneratorFactory.h"

using namespace ev3;

Ev3GeneratorFactory::Ev3GeneratorFactory(const qrRepo::RepoApi &repo
		, qReal::ErrorReporterInterface &errorReporter
		, const interpreterBase::robotModel::RobotModelManagerInterface &robotModelManager
		, generatorBase::lua::LuaProcessor &luaProcessor
		, const QString &generatorName)
	: GeneratorFactoryBase(repo, errorReporter, robotModelManager, luaProcessor)
	, mGeneratorName(generatorName)
{
}

Ev3GeneratorFactory::~Ev3GeneratorFactory()
{
}

generatorBase::simple::AbstractSimpleGenerator *Ev3GeneratorFactory::simpleGenerator(const qReal::Id &id, generatorBase::GeneratorCustomizer &customizer)
{
	Q_UNUSED(id)
	Q_UNUSED(customizer)
	return nullptr;
}

QString Ev3GeneratorFactory::pathToTemplates() const
{
	return "";
}

generatorBase::parts::DeviceVariables *Ev3GeneratorFactory::deviceVariables() const
{
	return nullptr;
}
