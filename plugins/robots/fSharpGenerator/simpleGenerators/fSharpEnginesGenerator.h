#pragma once

#include <simpleGenerators/bindingGenerator.h>

namespace qReal {
namespace robots {
namespace generators {
namespace simple {

/// Generator for 'EnginesForward' and 'EnginesBackward' block
class FSharpEnginesGenerator : public BindingGenerator
{
public:
    FSharpEnginesGenerator(qrRepo::RepoApi const &repo
			, GeneratorCustomizer &customizer
			, Id const &id
			, QString const &engineType
			, QObject *parent);
};

}
}
}
}
