#pragma once

#include "generatorBase/simpleGenerators/bindingGenerator.h"

namespace generatorBase {
namespace simple {

/// Generator for 'FinalNode' block
class FinalNodeGenerator : public BindingGenerator
{
public:
	FinalNodeGenerator(qrRepo::RepoApi const &repo
			, GeneratorCustomizer &customizer
			, qReal::Id const &id
			, bool inMainDiagram
			, QObject *parent = 0);
};

}
}
