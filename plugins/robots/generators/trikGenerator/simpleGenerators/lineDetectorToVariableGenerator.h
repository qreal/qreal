#pragma once

#include <generatorBase/simpleGenerators/bindingGenerator.h>

namespace trik {
namespace simple {

/// Generator for "line detector reading to variable" block.
class LineDetectorToVariableGenerator : public generatorBase::simple::BindingGenerator
{
public:
	LineDetectorToVariableGenerator(qrRepo::RepoApi const &repo
			, generatorBase::GeneratorCustomizer &customizer
			, qReal::Id const &id
			, QObject *parent);
};

}
}
