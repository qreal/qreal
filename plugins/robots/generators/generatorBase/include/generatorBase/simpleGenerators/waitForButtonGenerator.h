#pragma once

#include "generatorBase/simpleGenerators/bindingGenerator.h"
#include "generatorBase/robotsGeneratorDeclSpec.h"

namespace generatorBase {
namespace simple {

/// Generator for various waitings for button press.
class ROBOTS_GENERATOR_EXPORT WaitForButtonGenerator : public generatorBase::simple::BindingGenerator
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
