#pragma once

#include <generatorBase/generatorCustomizer.h>
#include "trikGeneratorFactory.h"

namespace qReal {
namespace robots {
namespace generators {
namespace trik {

class TrikGeneratorCustomizer : public GeneratorCustomizer
{
public:
	TrikGeneratorCustomizer(qrRepo::RepoApi const &repo
			, ErrorReporterInterface &errorReporter);

	virtual GeneratorFactoryBase *factory();

private:
	TrikGeneratorFactory mFactory;
};

}
}
}
}
