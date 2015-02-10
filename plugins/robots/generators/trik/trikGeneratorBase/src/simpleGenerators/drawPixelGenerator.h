#pragma once

#include <generatorBase/simpleGenerators/bindingGenerator.h>

namespace trik {
namespace simple {

/// Generator for Draw Pixel block.
class DrawPixelGenerator : public generatorBase::simple::BindingGenerator
{
public:
	DrawPixelGenerator(const qrRepo::RepoApi &repo
			, generatorBase::GeneratorCustomizer &customizer
			, const qReal::Id &id
			, QObject *parent);
};

}
}
