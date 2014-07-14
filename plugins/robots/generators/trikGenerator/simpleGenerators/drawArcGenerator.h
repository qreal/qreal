#pragma once

#include <generatorBase/simpleGenerators/bindingGenerator.h>

namespace trik {
namespace simple {

/// Generator for Draw Arc block.
class DrawArcGenerator : public generatorBase::simple::BindingGenerator
{
public:
    DrawArcGenerator(qrRepo::RepoApi const &repo
                     , generatorBase::GeneratorCustomizer &customizer
                     , qReal::Id const &id
                     , QObject *parent);
};

}
}
