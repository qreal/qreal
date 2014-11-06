#pragma once

#include <generatorBase/simpleGenerators/bindingGenerator.h>

namespace trik {
namespace simple {

/// Generator for 'SetBackground' block.
class SetBackgroundGenerator : public generatorBase::simple::BindingGenerator
{
public:
	SetBackgroundGenerator(qrRepo::RepoApi const &repo
			, generatorBase::GeneratorCustomizer &customizer
			, qReal::Id const &id
			, QObject *parent);
};

}
}
