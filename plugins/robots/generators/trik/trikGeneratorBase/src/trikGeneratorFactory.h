#pragma once

#include <generatorBase/generatorFactoryBase.h>

namespace trik {

/// Generator factory implemtation for TRIK platform
class TrikGeneratorFactory : public generatorBase::GeneratorFactoryBase
{
public:
	TrikGeneratorFactory(const qrRepo::RepoApi &repo
			, qReal::ErrorReporterInterface &errorReporter
			, const interpreterBase::robotModel::RobotModelManagerInterface &robotModelManager
			, generatorBase::lua::LuaProcessor &luaProcessor
			, const QString &generatorName);

	~TrikGeneratorFactory() override;

	generatorBase::simple::AbstractSimpleGenerator *simpleGenerator(const qReal::Id &id
			, generatorBase::GeneratorCustomizer &customizer) override;

	QString pathToTemplates() const override;

	generatorBase::parts::DeviceVariables *deviceVariables() const override;

private:
	const QString mGeneratorName;
};

}
