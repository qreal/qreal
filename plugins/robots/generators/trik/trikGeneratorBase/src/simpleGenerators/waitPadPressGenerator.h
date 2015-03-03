#pragma once

#include <generatorBase/simpleGenerators/bindingGenerator.h>

namespace trik {
namespace simple {

/// Generator for 'TrikWaitPadPress' block
class WaitPadPressGenerator : public generatorBase::simple::BindingGenerator
{
public:
	WaitPadPressGenerator(const qrRepo::RepoApi &repo
			, generatorBase::GeneratorCustomizer &customizer
			, const qReal::Id &id
			, QObject *parent = 0);
};

}
}
