#pragma once

#include <generatorFactoryBase.h>

namespace qReal {
namespace robots {
namespace generators {
namespace fSharp {

class FSharpGeneratorFactory : public GeneratorFactoryBase
{
public:
    FSharpGeneratorFactory(qrRepo::RepoApi const &repo
            , ErrorReporterInterface &errorReporter);
    virtual ~FSharpGeneratorFactory();

    virtual QString pathToTemplates() const;

};

}
}
}
}
