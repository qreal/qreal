#pragma once

#include "generatorBase/simpleGenerators/bindingGenerator.h"

namespace generatorBase {
namespace simple {

/// Generator for 'WaitForGyroscope' block
class WaitForGyroscopeBlockGenerator : public BindingGenerator
{
public:
	WaitForGyroscopeBlockGenerator(qrRepo::RepoApi const &repo
			, GeneratorCustomizer &customizer
			, qReal::Id const &id
			, QObject *parent = 0);
};

}
}
