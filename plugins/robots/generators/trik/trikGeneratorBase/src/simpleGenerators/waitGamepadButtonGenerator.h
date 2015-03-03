#pragma once

#include <generatorBase/simpleGenerators/bindingGenerator.h>

namespace trik {
namespace simple {

/// Generator for 'TrikWaitGamepadButton' block.
class WaitGamepadButtonGenerator : public generatorBase::simple::BindingGenerator
{
public:
	WaitGamepadButtonGenerator(const qrRepo::RepoApi &repo
			, generatorBase::GeneratorCustomizer &customizer
			, const qReal::Id &id
			, QObject *parent);
};

}
}
