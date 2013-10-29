#pragma once

#include "base/simpleGenerators/bindingGenerator.h"

namespace qReal {
namespace robots {
namespace generators {
namespace simple {

/// Generator for 'SetBackground' block.
class SetBackgroundGenerator : public BindingGenerator
{
public:
	SetBackgroundGenerator(qrRepo::RepoApi const &repo
			, GeneratorCustomizer &customizer
			, Id const &id
			, QObject *parent);
};

}
}
}
}
