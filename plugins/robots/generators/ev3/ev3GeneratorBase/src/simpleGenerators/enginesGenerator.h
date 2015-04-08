#pragma once

#include <generatorBase/simpleGenerators/bindingGenerator.h>

namespace ev3 {
namespace simple {

/// Generator for 'EnginesForward' and 'EnginesBackward' block
class EnginesGenerator : public generatorBase::simple::BindingGenerator
{
public:
	EnginesGenerator(qrRepo::RepoApi const &repo
			, generatorBase::GeneratorCustomizer &customizer
			, qReal::Id const &id
			, QString const &engineType
			, QObject *parent);
};

}
}
