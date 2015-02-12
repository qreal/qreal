#pragma once

#include "generatorBase/simpleGenerators/bindingGenerator.h"

namespace generatorBase {
namespace simple {

/// Generator for 'EnginesForward' and 'EnginesBackward' block
class EnginesGenerator : public BindingGenerator
{
public:
	EnginesGenerator(const qrRepo::RepoApi &repo
			, GeneratorCustomizer &customizer
			, const qReal::Id &id
			, const QString &engineType
			, QObject *parent);
};

}
}
