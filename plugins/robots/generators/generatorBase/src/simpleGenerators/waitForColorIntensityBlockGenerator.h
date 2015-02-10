#pragma once

#include "generatorBase/simpleGenerators/bindingGenerator.h"

namespace generatorBase {
namespace simple {

/// Generator for 'WaitForColorIntensity' block
class WaitForColorIntensityBlockGenerator : public BindingGenerator
{
public:
	WaitForColorIntensityBlockGenerator(const qrRepo::RepoApi &repo
			, GeneratorCustomizer &customizer
			, const qReal::Id &id
			, QObject *parent = 0);
};

}
}
