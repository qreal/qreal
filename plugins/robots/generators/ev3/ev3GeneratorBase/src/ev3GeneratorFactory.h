#pragma once

#include <generatorBase/generatorFactoryBase.h>

#include "ev3GeneratorBase/ev3GeneratorBaseDeclSpec.h"

namespace ev3 {

class ROBOTS_EV3_GENERATOR_BASE_EXPORT Ev3GeneratorFactory : public generatorBase::GeneratorFactoryBase
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

	generatorBase::simple::Binding::ConverterInterface *outputPortNameConverter() const;

	generatorBase::simple::Binding::ConverterInterface *goToBlockNumberConverter() const;

	QString pathToTemplates() const override;

private:
	QString const mGeneratorName;
	int mGoToBlockNumber;
};

}

