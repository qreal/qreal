#pragma once

#include <generatorBase/generatorFactoryBase.h>

namespace trik {

/// Generator factory implemtation for TRIK platform
class TrikQtsGeneratorFactory : public generatorBase::GeneratorFactoryBase
{
public:
	TrikQtsGeneratorFactory(qrRepo::RepoApi const &repo
			, qReal::ErrorReporterInterface &errorReporter
			, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager
			, generatorBase::lua::LuaProcessor &luaProcessor);

	~TrikQtsGeneratorFactory() override;

	generatorBase::simple::AbstractSimpleGenerator *simpleGenerator(qReal::Id const &id
			, generatorBase::GeneratorCustomizer &customizer) override;

	QString pathToTemplates() const override;

	generatorBase::simple::Binding::ConverterInterface *stringPropertyConverter() const override;

	generatorBase::parts::DeviceVariables *deviceVariables() const override;

protected:
	void initVariables() override;
};

}
