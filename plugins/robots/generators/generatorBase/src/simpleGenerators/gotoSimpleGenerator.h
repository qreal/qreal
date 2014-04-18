#pragma once

#include "generatorBase/simpleGenerators/bindingGenerator.h"

namespace qReal {
namespace robots {
namespace generators {
namespace simple {

/// Generator for 'goto' construction
class GotoSimpleGenerator : public BindingGenerator
{
public:
	GotoSimpleGenerator(qrRepo::RepoApi const &repo
			, GeneratorCustomizer &customizer
			, Id const &id
			, QObject *parent = 0);
};

}
}
}
}
