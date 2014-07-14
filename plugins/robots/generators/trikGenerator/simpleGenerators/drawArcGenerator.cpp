#include "drawArcGenerator.h"
#include <generatorBase/generatorCustomizer.h>

using namespace trik::simple;
using namespace generatorBase::simple;

DrawArcGenerator::DrawArcGenerator(qrRepo::RepoApi const &repo
    , generatorBase::GeneratorCustomizer &customizer
    , qReal::Id const &id
    , QObject *parent)
: BindingGenerator(repo, customizer, id, "drawing/drawArc.t", QList<Binding *>()
     << Binding::createConverting("@@XCoordinateArc@@", "XCoordinateArc", customizer.factory()->intPropertyConverter())
     << Binding::createConverting("@@YCoordinateArc@@", "YCoordinateArc", customizer.factory()->intPropertyConverter())
     << Binding::createConverting("@@WidthArc@@", "WidthArc", customizer.factory()->intPropertyConverter())
     << Binding::createConverting("@@HeightArc@@", "HeightArc", customizer.factory()->intPropertyConverter())
     << Binding::createConverting("@@StartAngle@@", "StartAngle", customizer.factory()->intPropertyConverter())
     << Binding::createConverting("@@SpanAngle@@", "SpanAngle", customizer.factory()->intPropertyConverter())
     , parent)
{
}
