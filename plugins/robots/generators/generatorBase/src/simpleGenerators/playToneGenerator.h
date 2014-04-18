#pragma once

#include "generatorBase/simpleGenerators/bindingGenerator.h"

namespace qReal {
namespace robots {
namespace generators {
namespace simple {

/// Generator for 'PlayTone' block
class PlayToneGenerator : public BindingGenerator
{
public:
	PlayToneGenerator(qrRepo::RepoApi const &repo
			, GeneratorCustomizer &customizer
			, Id const &id
			, QObject *parent = 0);
};

}
}
}
}
