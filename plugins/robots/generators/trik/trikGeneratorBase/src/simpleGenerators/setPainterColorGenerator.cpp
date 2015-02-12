#include "setPainterColorGenerator.h"

#include <generatorBase/generatorCustomizer.h>
#include "src/converters/backgroundColorConverter.h"

using namespace trik::simple;
using namespace trik::converters;
using namespace generatorBase::simple;

SetPainterColorGenerator::SetPainterColorGenerator(const qrRepo::RepoApi &repo
		, generatorBase::GeneratorCustomizer &customizer
		, const qReal::Id &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "drawing/setPainterColor.t", QList<Binding *>()
			<< Binding::createConverting("@@Color@@", "Color", new BackgroundColorConverter(customizer.factory()->pathToTemplates()))
			, parent)
{
}
