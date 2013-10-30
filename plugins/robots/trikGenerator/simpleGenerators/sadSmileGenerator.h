#pragma once

#include "simpleGenerators/bindingGenerator.h"

namespace qReal {
namespace robots {
namespace generators {
namespace simple {

/// Generator for 'SadSmile' block.
class SadSmileGenerator : public BindingGenerator
{
public:
	SadSmileGenerator(qrRepo::RepoApi const &repo
			, GeneratorCustomizer &customizer
			, Id const &id
			, QObject *parent);
};

}
}
}
}
