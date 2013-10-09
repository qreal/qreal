#pragma once

#include "base/generatorFactoryBase.h"

namespace qReal {
namespace robots {
namespace generators {
namespace trik {

class TrikGeneratorFactory : public GeneratorFactoryBase
{
public:
	TrikGeneratorFactory(qrRepo::RepoApi const &repo
			, ErrorReporterInterface &errorReporter);

	virtual ~TrikGeneratorFactory();

	virtual simple::AbstractSimpleGenerator *simpleGenerator(Id const &id
			, GeneratorCustomizer &customizer);

	virtual QString pathToTemplates() const;

	virtual simple::Binding::MultiConverterInterface *enginesConverter() const;

	virtual simple::Binding::ConverterInterface *inputPortConverter() const;

	virtual simple::Binding::ConverterInterface *outputPortConverter() const;

protected:
	virtual void initVariables();
};

}
}
}
}
