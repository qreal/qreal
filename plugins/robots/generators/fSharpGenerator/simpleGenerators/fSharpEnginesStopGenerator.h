#pragma once

#include <generatorBase/simpleGenerators/bindingGenerator.h>

namespace fSharp {
namespace simple {

/// Generator for 'EnginesStop' block
class FSharpEnginesStopGenerator : public generatorBase::simple::BindingGenerator
{
public:
	FSharpEnginesStopGenerator(qrRepo::RepoApi const &repo
			, generatorBase::GeneratorCustomizer &customizer
			, qReal:: Id const &id
			, QObject *parent = 0);
};

}
}
