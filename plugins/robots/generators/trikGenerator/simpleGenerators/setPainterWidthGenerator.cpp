#include "setPainterWidthGenerator.h"
#include <generatorBase/generatorCustomizer.h>

using namespace trik::simple;
using namespace generatorBase::simple;

SetPainterWidthGenerator::SetPainterWidthGenerator(qrRepo::RepoApi const &repo
		, generatorBase::GeneratorCustomizer &customizer
		, qReal::Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "drawing/setPainterWidth.t", QList<Binding *>()
			<< Binding::createConverting("@@Width@@", "Width", customizer.factory()->intPropertyConverter())
			, parent)
{
}
