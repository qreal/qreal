#pragma once

#include <generatorBase/simpleGenerators/bindingGenerator.h>

namespace trik {
namespace simple {

/// Generator for 'TrikWaitGamepadWheel' block
class WaitGamepadWheelGenerator : public generatorBase::simple::BindingGenerator
{
public:
	WaitGamepadWheelGenerator(const qrRepo::RepoApi &repo
			, generatorBase::GeneratorCustomizer &customizer
			, const qReal::Id &id
			, QObject *parent = 0);
};

}
}
