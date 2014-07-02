#pragma once

#include "generatorBase/simpleGenerators/bindingGenerator.h"

namespace generatorBase {
namespace simple {

/// Generator for 'EnginesForward' and 'EnginesBackward' block
class EnginesGenerator : public BindingGenerator
{
public:
	EnginesGenerator(qrRepo::RepoApi const &repo
			, GeneratorCustomizer &customizer
			, qReal::Id const &id
			, QString const &engineType
			, QObject *parent);
};

}
}
