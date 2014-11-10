#pragma once

#include <generatorBase/simpleGenerators/bindingGenerator.h>

namespace trik {
namespace simple {

/// Generator for "detect line" block.
class DetectGenerator : public generatorBase::simple::BindingGenerator
{
public:
	DetectGenerator(qrRepo::RepoApi const &repo
			, generatorBase::GeneratorCustomizer &customizer
			, qReal::Id const &id
			, QObject *parent);
};

}
}
