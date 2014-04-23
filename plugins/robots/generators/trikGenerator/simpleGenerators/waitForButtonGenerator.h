#pragma once

#include <generatorBase/simpleGenerators/bindingGenerator.h>

namespace trik {
namespace simple {

/// Generator for various waitings for button press.
class WaitForButtonGenerator : public generatorBase::simple::BindingGenerator
{
public:
	WaitForButtonGenerator(qrRepo::RepoApi const &repo
			, generatorBase::GeneratorCustomizer &customizer
			, qReal::Id const &id
			, QString const &buttonTemplate
			, QObject *parent);
};

}
}
