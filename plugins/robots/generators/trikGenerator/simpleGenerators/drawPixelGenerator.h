#pragma once

#include <generatorBase/simpleGenerators/bindingGenerator.h>

namespace trik {
namespace simple {

/// Generator for Draw Pixel block.
class DrawPixelGenerator : public generatorBase::simple::BindingGenerator
{
public:
    DrawPixelGenerator(qrRepo::RepoApi const &repo
                       , generatorBase::GeneratorCustomizer &customizer
                       , qReal::Id const &id
                       , QObject *parent);
};

}
}

