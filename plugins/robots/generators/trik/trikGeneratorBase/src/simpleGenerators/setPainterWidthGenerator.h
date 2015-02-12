#pragma once

#include <generatorBase/simpleGenerators/bindingGenerator.h>

namespace trik {
namespace simple {

/// Generator for Set Painter Width block.
class SetPainterWidthGenerator : public generatorBase::simple::BindingGenerator
{
public:
	SetPainterWidthGenerator(const qrRepo::RepoApi &repo
			, generatorBase::GeneratorCustomizer &customizer
			, const qReal::Id &id
			, QObject *parent);
};

}
}
