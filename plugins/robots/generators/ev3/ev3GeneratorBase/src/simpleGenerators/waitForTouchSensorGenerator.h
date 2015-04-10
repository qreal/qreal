#pragma once

#include <generatorBase/simpleGenerators/bindingGenerator.h>

namespace ev3 {
namespace simple {

/// Generator for 'WaitForTouchSensor' block
class WaitForTouchSensorGenerator : public generatorBase::simple::BindingGenerator
{
public:
	WaitForTouchSensorGenerator(qrRepo::RepoApi const &repo
			, generatorBase::GeneratorCustomizer &customizer
			, qReal::Id const &id
			, QObject *parent);
};

}
}
