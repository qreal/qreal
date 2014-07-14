#include "drawLineGenerator.h"
#include <generatorBase/generatorCustomizer.h>

using namespace trik::simple;
using namespace generatorBase::simple;

DrawLineGenerator::DrawLineGenerator(qrRepo::RepoApi const &repo
    , generatorBase::GeneratorCustomizer &customizer
    , qReal::Id const &id
    , QObject *parent)
: BindingGenerator(repo, customizer, id, "drawing/drawLine.t", QList<Binding *>()
     << Binding::createConverting("@@X1CoordinateLine@@", "X1CoordinateLine", customizer.factory()->intPropertyConverter())
     << Binding::createConverting("@@Y1CoordinateLine@@", "Y1CoordinateLine", customizer.factory()->intPropertyConverter())
     << Binding::createConverting("@@X2CoordinateLine@@", "X2CoordinateLine", customizer.factory()->intPropertyConverter())
     << Binding::createConverting("@@Y2CoordinateLine@@", "Y2CoordinateLine", customizer.factory()->intPropertyConverter())
     , parent)
{
}
