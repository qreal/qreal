#pragma once

#include "generatorBase/simpleGenerators/bindingGenerator.h"

namespace generatorBase {
namespace simple {

/// Generator for subprogram call block
class SubprogramsSimpleGenerator : public BindingGenerator
{
public:
	SubprogramsSimpleGenerator(qrRepo::RepoApi const &repo
			, GeneratorCustomizer &customizer
			, qReal::Id const &id
			, QObject *parent = 0);

	virtual QString generate();
};

}
}
