#pragma once

#include <generatorBase/generatorCustomizer.h>
#include "russianCGeneratorFactory.h"

namespace russianC {

class RussianCGeneratorCustomizer : public generatorBase::GeneratorCustomizer
{
public:
	RussianCGeneratorCustomizer(qrRepo::RepoApi const &repo
			, qReal::ErrorReporterInterface &errorReporter);

	generatorBase::GeneratorFactoryBase *factory() override;

private:
	RussianCGeneratorFactory *mFactory;
};

}
