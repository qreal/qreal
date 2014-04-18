#pragma once

#include <generatorBase/simpleGenerators/bindingGenerator.h>

namespace qReal {
namespace robots {
namespace generators {
namespace simple {

/// Generator for 'EnginesForward' and 'EnginesBackward' block
class TrikEnginesGenerator : public BindingGenerator
{
public:
	TrikEnginesGenerator(qrRepo::RepoApi const &repo
			, GeneratorCustomizer &customizer
			, Id const &id
			, QString const &engineType
			, QObject *parent);
};

}
}
}
}
