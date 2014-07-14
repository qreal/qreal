#pragma once

#include <generatorBase/simpleGenerators/bindingGenerator.h>

namespace trik {
namespace simple {

/// Generator for Set Painter Width block.
class SetPainterWidthGenerator : public generatorBase::simple::BindingGenerator
{
public:
    SetPainterWidthGenerator(qrRepo::RepoApi const &repo
                             , generatorBase::GeneratorCustomizer &customizer
                             , qReal::Id const &id
                             , QObject *parent);
};

}
}
