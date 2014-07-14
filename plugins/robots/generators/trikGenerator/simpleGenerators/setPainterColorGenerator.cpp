#include "setPainterColorGenerator.h"

#include <generatorBase/generatorCustomizer.h>
#include "converters/backgroundColorConverter.h"

using namespace trik::simple;
using namespace trik::converters;
using namespace generatorBase::simple;

SetPainterColorGenerator::SetPainterColorGenerator(qrRepo::RepoApi const &repo
    , generatorBase::GeneratorCustomizer &customizer
    , qReal::Id const &id
    , QObject *parent)
: BindingGenerator(repo, customizer, id, "drawing/setPainterColor.t", QList<Binding *>()
     << Binding::createConverting("@@Color@@", "Color", new BackgroundColorConverter(customizer.factory()->pathToTemplates()))
     , parent)
{
}
