#pragma once

#include "generatorBase/simpleGenerators/bindingGenerator.h"

namespace qReal {
namespace robots {
namespace generators {
namespace simple {

/// Generator for 'WaitForColor' block
class WaitForColorBlockGenerator : public BindingGenerator
{
public:
	WaitForColorBlockGenerator(qrRepo::RepoApi const &repo
			, GeneratorCustomizer &customizer
			, Id const &id
			, QObject *parent = 0);
};

}
}
}
}
