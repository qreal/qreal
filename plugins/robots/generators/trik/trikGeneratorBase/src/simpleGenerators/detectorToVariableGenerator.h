#pragma once

#include <generatorBase/simpleGenerators/bindingGenerator.h>

namespace trik {
namespace simple {

/// Generator for "line detector reading to variable" block.
class DetectorToVariableGenerator : public generatorBase::simple::BindingGenerator
{
public:
	DetectorToVariableGenerator(const qrRepo::RepoApi &repo
			, generatorBase::GeneratorCustomizer &customizer
			, const qReal::Id &id
			, QObject *parent);
};

}
}
