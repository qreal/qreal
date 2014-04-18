#pragma once

#include <generatorBase/simpleGenerators/bindingGenerator.h>

namespace qReal {
namespace robots {
namespace generators {
namespace simple {

/// Generator for 'EnginesStop' block
class TrikEnginesStopGenerator : public BindingGenerator
{
public:
	TrikEnginesStopGenerator(qrRepo::RepoApi const &repo
			, GeneratorCustomizer &customizer
			, Id const &id
			, QObject *parent = 0);
};

}
}
}
}
