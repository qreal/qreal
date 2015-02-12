#pragma once

#include "generatorBase/simpleGenerators/bindingGenerator.h"

namespace generatorBase {
namespace simple {

/// Generator for 'FinalNode' block
class FinalNodeGenerator : public BindingGenerator
{
public:
	FinalNodeGenerator(const qrRepo::RepoApi &repo
			, GeneratorCustomizer &customizer
			, const qReal::Id &id
			, bool inMainDiagram
			, QObject *parent = 0);
};

}
}
