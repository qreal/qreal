#pragma once

#include <generatorCustomizer.h>
#include "fSharpGeneratorFactory.h"

namespace qReal {
namespace robots {
namespace generators {
namespace fSharp {

/// Generator customizer for F# language generation process
class FSharpGeneratorCustomizer : public GeneratorCustomizer
{
public:
    FSharpGeneratorCustomizer(qrRepo::RepoApi const &repo
            , ErrorReporterInterface &errorReporter);

    GeneratorFactoryBase *factory() override;

private:
    FSharpGeneratorFactory mFactory;
};

}
}
}
}
