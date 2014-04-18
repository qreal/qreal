#pragma once

#include <generatorBase/generatorCustomizer.h>
#include "russianCGeneratorFactory.h"

namespace qReal {
namespace robots {
namespace generators {
namespace russianC {

class RussianCGeneratorCustomizer : public GeneratorCustomizer
{
public:
	RussianCGeneratorCustomizer(qrRepo::RepoApi const &repo
			, ErrorReporterInterface &errorReporter);

	virtual GeneratorFactoryBase *factory();

private:
	RussianCGeneratorFactory *mFactory;
};

}
}
}
}
