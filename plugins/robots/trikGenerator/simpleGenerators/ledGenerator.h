#pragma once

#include "simpleGenerators/bindingGenerator.h"

namespace qReal {
namespace robots {
namespace generators {
namespace simple {

/// Generator for LED control block.
class LedGenerator : public BindingGenerator
{
public:
	LedGenerator(qrRepo::RepoApi const &repo
			, GeneratorCustomizer &customizer
			, Id const &id
			, QObject *parent);
};

}
}
}
}
