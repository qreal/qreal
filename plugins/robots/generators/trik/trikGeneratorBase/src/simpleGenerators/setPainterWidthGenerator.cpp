#include "setPainterWidthGenerator.h"
#include <generatorBase/generatorCustomizer.h>

using namespace trik::simple;
using namespace generatorBase::simple;

SetPainterWidthGenerator::SetPainterWidthGenerator(const qrRepo::RepoApi &repo
		, generatorBase::GeneratorCustomizer &customizer
		, const qReal::Id &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "drawing/setPainterWidth.t", QList<Binding *>()
			<< Binding::createConverting("@@Width@@", "Width"
					, customizer.factory()->intPropertyConverter(id, "Width"))
			, parent)
{
}
