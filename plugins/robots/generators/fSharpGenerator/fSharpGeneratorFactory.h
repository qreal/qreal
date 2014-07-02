#pragma once

#include <generatorBase/generatorFactoryBase.h>

namespace fSharp {

/// Generator factory implemtation for TRIK platform
class FSharpGeneratorFactory : public generatorBase::GeneratorFactoryBase
{
public:
	FSharpGeneratorFactory(qrRepo::RepoApi const &repo
			, qReal::ErrorReporterInterface &errorReporter
			, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager);

	~FSharpGeneratorFactory() override;

	generatorBase::simple::AbstractSimpleGenerator *simpleGenerator(qReal::Id const &id
			, generatorBase::GeneratorCustomizer &customizer) override;

	QString pathToTemplates() const override;

	generatorBase::simple::Binding::MultiConverterInterface *enginesConverter() const override;
	generatorBase::simple::Binding::MultiConverterInterface *encodersConverter() const;

	generatorBase::simple::Binding::ConverterInterface *inputPortConverter() const override;
	generatorBase::simple::Binding::ConverterInterface *outputPortConverter() const override;

	generatorBase::simple::Binding::ConverterInterface *stringPropertyConverter() const override;

protected:
	void initVariables() override;

	QString motorPortSplitRegexp() const;
	generatorBase::simple::Binding::ConverterInterface *motorPortConverter() const;
	generatorBase::simple::Binding::ConverterInterface *encoderPortConverter() const;
};

}
