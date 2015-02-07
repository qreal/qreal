#pragma once

#include <generatorBase/simpleGenerators/bindingGenerator.h>

namespace trik {
namespace simple {

/// Generator for 'TrikWaitGamepadDisconnect' block
class WaitGamepadDisconnectGenerator : public generatorBase::simple::BindingGenerator
{
public:
	WaitGamepadDisconnectGenerator(const qrRepo::RepoApi &repo
			, generatorBase::GeneratorCustomizer &customizer
			, const qReal::Id &id
			, QObject *parent = 0);
};

}
}
