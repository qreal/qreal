#pragma once

#include <generatorCustomizer.h>
#include "nxtOsekGeneratorFactory.h"

namespace qReal {
namespace robots {
namespace generators {
namespace nxtOsek {

class NxtOsekGeneratorCustomizer : public GeneratorCustomizer
{
public:
	NxtOsekGeneratorCustomizer(qrRepo::RepoApi const &repo
			, ErrorReporterInterface &errorReporter);

	virtual GeneratorFactoryBase *factory();

private:
	NxtOsekGeneratorFactory mFactory;
};

}
}
}
}
