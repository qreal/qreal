#pragma once

#include <generatorBase/generatorFactoryBase.h>

namespace nxtOsek {

class NxtOsekGeneratorFactory : public generatorBase::GeneratorFactoryBase
{
public:
	NxtOsekGeneratorFactory(qrRepo::RepoApi const &repo
			, qReal::ErrorReporterInterface &errorReporter
			, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager);
	~NxtOsekGeneratorFactory() override;

	generatorBase::simple::AbstractSimpleGenerator *simpleGenerator(qReal::Id const &id
			, generatorBase::GeneratorCustomizer &customizer) override;

	QString pathToTemplates() const override;

	generatorBase::simple::Binding::ConverterInterface *stringPropertyConverter() const override;
};

}
