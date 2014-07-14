#include "drawPixelGenerator.h"
#include <generatorBase/generatorCustomizer.h>

using namespace trik::simple;
using namespace generatorBase::simple;

DrawPixelGenerator::DrawPixelGenerator(qrRepo::RepoApi const &repo
    , generatorBase::GeneratorCustomizer &customizer
    , qReal::Id const &id
    , QObject *parent)
: BindingGenerator(repo, customizer, id, "drawing/drawPixel.t", QList<Binding *>()
     << Binding::createConverting("@@XCoordinatePix@@", "XCoordinatePix", customizer.factory()->intPropertyConverter())
     << Binding::createConverting("@@YCoordinatePix@@", "YCoordinatePix", customizer.factory()->intPropertyConverter())
     , parent)
{
}
