#pragma once

#include <masterGeneratorBase.h>

namespace qReal {
namespace robots {
namespace generators {
namespace fSharp {

/// Master generator implementation for F# language
class FSharpMasterGenerator : public MasterGeneratorBase
{
public:
    FSharpMasterGenerator(qrRepo::RepoApi const &repo
            , ErrorReporterInterface &errorReporter
            , Id const &diagramId);

private:
    GeneratorCustomizer *createCustomizer() override;
    QString targetPath() override;
    bool supportsGotoGeneration() const override;
};

}
}
}
}
