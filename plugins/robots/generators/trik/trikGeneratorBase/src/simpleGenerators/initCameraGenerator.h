#pragma once

#include <generatorBase/simpleGenerators/bindingGenerator.h>

namespace trik {
namespace simple {

/// Generator for "init camera" block.
class InitCameraGenerator : public generatorBase::simple::BindingGenerator
{
public:
	InitCameraGenerator(const qrRepo::RepoApi &repo
			, generatorBase::GeneratorCustomizer &customizer
			, const qReal::Id &id
			, QObject *parent);
};

}
}
