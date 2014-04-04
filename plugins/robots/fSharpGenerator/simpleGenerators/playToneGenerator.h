#pragma once

#include <simpleGenerators/bindingGenerator.h>

namespace qReal {
namespace robots {
namespace generators {
namespace simple {

/// Generator for 'PlayTone' block.
class PlayToneGenerator : public BindingGenerator
{
public:
	PlayToneGenerator(qrRepo::RepoApi const &repo
			, GeneratorCustomizer &customizer
			, Id const &id
			, QObject *parent);
};

}
}
}
}
