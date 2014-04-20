#pragma once

#include <generatorBase/simpleGenerators/bindingGenerator.h>

namespace trik {
namespace simple {

/// Generator for 'EnginesStop' block
class TrikEnginesStopGenerator : public generatorBase::simple::BindingGenerator
{
public:
	TrikEnginesStopGenerator(qrRepo::RepoApi const &repo
			, generatorBase::GeneratorCustomizer &customizer
			, qReal::Id const &id
			, QObject *parent = 0);
};

}
}
