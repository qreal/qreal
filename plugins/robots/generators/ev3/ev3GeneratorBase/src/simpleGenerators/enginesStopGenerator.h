#pragma once

#include <generatorBase/simpleGenerators/bindingGenerator.h>

namespace ev3 {
namespace simple {

/// Generator for 'EnginesStop' block
class EnginesStopGenerator : public generatorBase::simple::BindingGenerator
{
public:
	EnginesStopGenerator(qrRepo::RepoApi const &repo
			, generatorBase::GeneratorCustomizer &customizer
			, qReal::Id const &id
			, QObject *parent);
};

}
}
