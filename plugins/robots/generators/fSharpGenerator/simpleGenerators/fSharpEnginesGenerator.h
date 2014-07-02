#pragma once

#include <generatorBase/simpleGenerators/bindingGenerator.h>

namespace fSharp {
namespace simple {

/// Generator for 'EnginesForward', 'EnginesBackward' and 'AngularServo' block
class FSharpEnginesGenerator : public generatorBase::simple::BindingGenerator
{
public:
	FSharpEnginesGenerator(qrRepo::RepoApi const &repo
			, generatorBase::GeneratorCustomizer &customizer
			, qReal::Id const &id
			, QString const &engineType
			, QObject *parent);
};

}
}
