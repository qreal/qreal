#pragma once

#include "generatorBase/simpleGenerators/bindingGenerator.h"

namespace generatorBase {
namespace simple {

/// Generator for 'NullifyEncoder' block
class NullificationEncoderGenerator: public BindingGenerator
{
public:
	NullificationEncoderGenerator(qrRepo::RepoApi const &repo
			, GeneratorCustomizer &customizer
			, qReal::Id const &id
			, QObject *parent = 0);
};

}
}
