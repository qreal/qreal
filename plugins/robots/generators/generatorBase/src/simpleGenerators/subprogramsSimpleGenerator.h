#pragma once

#include "generatorBase/simpleGenerators/bindingGenerator.h"

namespace qReal {
namespace robots {
namespace generators {
namespace simple {

/// Generator for subprogram call block
class SubprogramsSimpleGenerator : public BindingGenerator
{
public:
	SubprogramsSimpleGenerator(qrRepo::RepoApi const &repo
			, GeneratorCustomizer &customizer
			, Id const &id
			, QObject *parent = 0);

	virtual QString generate();
};

}
}
}
}
