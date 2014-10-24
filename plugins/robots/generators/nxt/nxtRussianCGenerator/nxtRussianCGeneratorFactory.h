#pragma once

#include <generatorBase/generatorFactoryBase.h>

namespace russianC {

class NxtRussianCGeneratorFactory : public generatorBase::GeneratorFactoryBase
{
public:
	NxtRussianCGeneratorFactory(qrRepo::RepoApi const &repo
			, qReal::ErrorReporterInterface &errorReporter
			, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager
			, generatorBase::lua::LuaProcessor &luaProcessor);
	~NxtRussianCGeneratorFactory() override;

	generatorBase::simple::AbstractSimpleGenerator *simpleGenerator(qReal::Id const &id
			, generatorBase::GeneratorCustomizer &customizer) override;

	QString pathToTemplates() const override;

	generatorBase::simple::Binding::ConverterInterface *stringPropertyConverter() const override;
};

}
