#pragma once

#include "generatorBase/simpleGenerators/bindingGenerator.h"
#include "generatorBase/robotsGeneratorDeclSpec.h"

namespace generatorBase {
namespace simple {

/// Generator for various waitings for button press.
class ROBOTS_GENERATOR_EXPORT WaitForButtonGenerator : public generatorBase::simple::BindingGenerator
{
public:
	WaitForButtonGenerator(const qrRepo::RepoApi &repo
			, generatorBase::GeneratorCustomizer &customizer
			, const qReal::Id &id
			, const QString &buttonTemplate
			, QObject *parent);
};

}
}
