#pragma once

#include <generatorCustomizer.h>
#include "fSharpGeneratorFactory.h"

namespace qReal {
namespace robots {
namespace generators {
namespace fSharp {

class FSharpGeneratorCustomizer : public GeneratorCustomizer
{
public:
    FSharpGeneratorCustomizer(qrRepo::RepoApi const &repo
			, ErrorReporterInterface &errorReporter);

	virtual GeneratorFactoryBase *factory();

private:
    FSharpGeneratorFactory mFactory;
};

}
}
}
}
