#pragma once

#include "generatorBase/simpleGenerators/bindingGenerator.h"

namespace generatorBase {
namespace simple {

/// Generator for 'WaitForTouch' block
class WaitForTouchSensorBlockGenerator : public BindingGenerator
{
public:
	WaitForTouchSensorBlockGenerator(qrRepo::RepoApi const &repo
			, GeneratorCustomizer &customizer
			, qReal::Id const &id
			, QObject *parent = 0);
};

}
}
