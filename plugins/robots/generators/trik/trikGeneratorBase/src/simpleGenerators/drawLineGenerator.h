#pragma once

#include <generatorBase/simpleGenerators/bindingGenerator.h>

namespace trik {
namespace simple {

/// Generator for Draw Line block.
class DrawLineGenerator : public generatorBase::simple::BindingGenerator
{
public:
	DrawLineGenerator(const qrRepo::RepoApi &repo
			, generatorBase::GeneratorCustomizer &customizer
			, const qReal::Id &id
			, QObject *parent);
};

}
}
