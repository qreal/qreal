#pragma once

#include "generatorBase/simpleGenerators/bindingGenerator.h"

namespace qReal {
namespace robots {
namespace generators {
namespace simple {

/// Generator for 'break' construction
class BreakGenerator : public BindingGenerator
{
public:
	BreakGenerator(qrRepo::RepoApi const &repo
			, GeneratorCustomizer &customizer
			, Id const &id
			, QObject *parent = 0);
};

}
}
}
}
