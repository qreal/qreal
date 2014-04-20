#pragma once

#include <generatorBase/simpleGenerators/bindingGenerator.h>

namespace trik {
namespace simple {

/// Generator for 'EnginesForward' and 'EnginesBackward' block
class TrikEnginesGenerator : public generatorBase::simple::BindingGenerator
{
public:
	TrikEnginesGenerator(qrRepo::RepoApi const &repo
			, generatorBase::GeneratorCustomizer &customizer
			, qReal::Id const &id
			, QString const &engineType
			, QObject *parent);
};

}
}
