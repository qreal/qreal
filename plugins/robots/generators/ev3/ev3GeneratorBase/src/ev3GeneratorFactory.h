#pragma once

#include <generatorBase/generatorFactoryBase.h>

namespace ev3 {

class Ev3GeneratorFactory : public generatorBase::GeneratorFactoryBase
{
public:
	Ev3GeneratorFactory(qrRepo::RepoApi const &repo
			, qReal::ErrorReporterInterface &errorReporter
			, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager
			, generatorBase::lua::LuaProcessor &luaProcessor
			, QString const &generatorName);

	~Ev3GeneratorFactory() override;

	generatorBase::simple::AbstractSimpleGenerator *simpleGenerator(qReal::Id const &id
			, generatorBase::GeneratorCustomizer &customizer) override;

	QString pathToTemplates() const override;

	generatorBase::parts::DeviceVariables *deviceVariables() const override;

private:
	QString const mGeneratorName;
};

}

