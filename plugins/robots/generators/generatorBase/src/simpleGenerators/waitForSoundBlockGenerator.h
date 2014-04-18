#pragma once

#include "generatorBase/simpleGenerators/bindingGenerator.h"

namespace qReal {
namespace robots {
namespace generators {
namespace simple {

/// Generator for 'WaitForSound' block
class WaitForSoundBlockGenerator : public BindingGenerator
{
public:
	WaitForSoundBlockGenerator(qrRepo::RepoApi const &repo
			, GeneratorCustomizer &customizer
			, Id const &id
			, QObject *parent = 0);
};

}
}
}
}
