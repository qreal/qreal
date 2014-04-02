#pragma once

#include <simpleGenerators/bindingGenerator.h>

namespace qReal {
namespace robots {
namespace generators {
namespace simple {

/// Generator for 'Say' block.
class SayGenerator : public BindingGenerator
{
public:
	SayGenerator(qrRepo::RepoApi const &repo
			, GeneratorCustomizer &customizer
			, Id const &id
			, QObject *parent);
};

}
}
}
}
