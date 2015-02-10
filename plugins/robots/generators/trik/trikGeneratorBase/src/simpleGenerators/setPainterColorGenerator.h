#pragma once

#include <generatorBase/simpleGenerators/bindingGenerator.h>

namespace trik {
namespace simple {

/// Generator for Set Painter Color block.
class SetPainterColorGenerator : public generatorBase::simple::BindingGenerator
{
public:
	SetPainterColorGenerator(const qrRepo::RepoApi &repo
			, generatorBase::GeneratorCustomizer &customizer
			, const qReal::Id &id
			, QObject *parent);
};

}
}
