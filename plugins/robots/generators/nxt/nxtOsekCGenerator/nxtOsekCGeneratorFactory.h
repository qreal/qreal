#pragma once

#include <generatorBase/generatorFactoryBase.h>

namespace nxtOsek {

class NxtOsekCGeneratorFactory : public generatorBase::GeneratorFactoryBase
{
public:
	NxtOsekCGeneratorFactory(qrRepo::RepoApi const &repo
			, qReal::ErrorReporterInterface &errorReporter
			, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager
			, generatorBase::lua::LuaProcessor &luaProcessor);
	~NxtOsekCGeneratorFactory() override;

	generatorBase::simple::AbstractSimpleGenerator *simpleGenerator(qReal::Id const &id
			, generatorBase::GeneratorCustomizer &customizer) override;

	QString pathToTemplates() const override;

	generatorBase::simple::Binding::ConverterInterface *stringPropertyConverter() const override;
};

}
