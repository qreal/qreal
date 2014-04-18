#pragma once

#include <generatorBase/generatorFactoryBase.h>

namespace qReal {
namespace robots {
namespace generators {
namespace nxtOsek {

class NxtOsekGeneratorFactory : public GeneratorFactoryBase
{
public:
	NxtOsekGeneratorFactory(qrRepo::RepoApi const &repo
			, ErrorReporterInterface &errorReporter);
	virtual ~NxtOsekGeneratorFactory();

	virtual QString pathToTemplates() const;

	virtual simple::Binding::ConverterInterface *stringPropertyConverter() const;
};

}
}
}
}
