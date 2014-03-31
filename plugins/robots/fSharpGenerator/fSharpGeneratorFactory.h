#pragma once

#include <generatorFactoryBase.h>

namespace qReal {
namespace robots {
namespace generators {
namespace fSharp {

/// Generator factory implemtation for FSharp platform
class FSharpGeneratorFactory : public GeneratorFactoryBase
{
public:
    FSharpGeneratorFactory(qrRepo::RepoApi const &repo
			, ErrorReporterInterface &errorReporter);

    virtual ~FSharpGeneratorFactory();

	virtual simple::AbstractSimpleGenerator *simpleGenerator(Id const &id
			, GeneratorCustomizer &customizer);

	virtual QString pathToTemplates() const;

	simple::Binding::MultiConverterInterface *enginesConverter(bool powerMotors) const;
	simple::Binding::MultiConverterInterface *enginesConverter() const override;

	simple::Binding::ConverterInterface *inputPortConverter() const override;
	simple::Binding::ConverterInterface *outputPortConverter() const override;

protected:
	virtual void initVariables();
};

}
}
}
}
