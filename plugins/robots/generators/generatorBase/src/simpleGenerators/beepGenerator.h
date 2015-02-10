#pragma once

#include "generatorBase/simpleGenerators/bindingGenerator.h"

namespace generatorBase {
namespace simple {

/// Generator for 'Beep' block
class BeepGenerator : public BindingGenerator
{
public:
	BeepGenerator(const qrRepo::RepoApi &repo
			, GeneratorCustomizer &customizer
			, const qReal::Id &id
			, QObject *parent = 0);
};

}
}
