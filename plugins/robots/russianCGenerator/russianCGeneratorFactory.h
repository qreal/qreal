#pragma once

#include <generatorFactoryBase.h>

namespace qReal {
namespace robots {
namespace generators {
namespace russianC {

class RussianCGeneratorFactory : public GeneratorFactoryBase
{
public:
	RussianCGeneratorFactory(qrRepo::RepoApi const &repo
			, ErrorReporterInterface &errorReporter);
	virtual ~RussianCGeneratorFactory();

	virtual QString pathToTemplates() const;

	virtual simple::Binding::ConverterInterface *stringPropertyConverter() const;
};

}
}
}
}
