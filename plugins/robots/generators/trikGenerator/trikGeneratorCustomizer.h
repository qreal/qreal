#pragma once

#include <generatorBase/generatorCustomizer.h>
#include "trikGeneratorFactory.h"

namespace trik {

class TrikGeneratorCustomizer : public generatorBase::GeneratorCustomizer
{
public:
	TrikGeneratorCustomizer(qrRepo::RepoApi const &repo
			, qReal::ErrorReporterInterface &errorReporter);

	generatorBase::GeneratorFactoryBase *factory() override;

private:
	TrikGeneratorFactory mFactory;
};

}
