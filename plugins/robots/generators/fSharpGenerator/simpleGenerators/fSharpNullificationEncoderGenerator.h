#pragma once

#include <generatorBase/simpleGenerators/bindingGenerator.h>

namespace fSharp {
namespace simple {

/// Generator for 'NullifyEncoder' block for FSharp
class FSharpNullificationEncoderGenerator: public generatorBase::simple::BindingGenerator
{
public:
	FSharpNullificationEncoderGenerator(qrRepo::RepoApi const &repo
			, generatorBase::GeneratorCustomizer &customizer
			, qReal::Id const &id
			, QObject *parent = 0);
};

}
}
