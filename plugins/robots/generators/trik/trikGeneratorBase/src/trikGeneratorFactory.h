#pragma once

#include <generatorBase/generatorFactoryBase.h>

namespace trik {

/// Generator factory implemtation for TRIK platform
class TrikGeneratorFactory : public generatorBase::GeneratorFactoryBase
{
public:
	TrikGeneratorFactory(qrRepo::RepoApi const &repo
			, qReal::ErrorReporterInterface &errorReporter
			, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager
			, generatorBase::lua::LuaProcessor &luaProcessor
			, QString const &generatorName);

	~TrikGeneratorFactory() override;

	generatorBase::simple::AbstractSimpleGenerator *simpleGenerator(qReal::Id const &id
			, generatorBase::GeneratorCustomizer &customizer) override;

	QString pathToTemplates() const override;

	generatorBase::simple::Binding::ConverterInterface *stringPropertyConverter() const override;

	generatorBase::parts::DeviceVariables *deviceVariables() const override;

private:
	QString const mGeneratorName;
};

}
