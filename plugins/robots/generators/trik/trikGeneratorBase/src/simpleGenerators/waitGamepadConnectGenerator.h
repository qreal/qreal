#pragma once

#include <generatorBase/simpleGenerators/bindingGenerator.h>

namespace trik {
namespace simple {

/// Generator for 'TrikWaitGamepadConnect' block
class WaitGamepadConnectGenerator : public generatorBase::simple::BindingGenerator
{
public:
	WaitGamepadConnectGenerator(const qrRepo::RepoApi &repo
			, generatorBase::GeneratorCustomizer &customizer
			, const qReal::Id &id
			, QObject *parent = 0);
};

}
}
