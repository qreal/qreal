#pragma once

#include "generatorBase/simpleGenerators/bindingGenerator.h"

namespace generatorBase {
namespace simple {

/// Generator for 'WaitForEncoder' block
class WaitForEncoderBlockGenerator : public BindingGenerator
{
public:
	WaitForEncoderBlockGenerator(const qrRepo::RepoApi &repo
			, GeneratorCustomizer &customizer
			, const qReal::Id &id
			, QObject *parent = 0);
};

}
}
