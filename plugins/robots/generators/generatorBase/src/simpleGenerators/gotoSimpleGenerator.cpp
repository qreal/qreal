#include "gotoSimpleGenerator.h"

#include "generatorBase/generatorCustomizer.h"

using namespace generatorBase::simple;
using namespace qReal;

GotoSimpleGenerator::GotoSimpleGenerator(const qrRepo::RepoApi &repo
		, GeneratorCustomizer &customizer
		, const Id &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "goto.t"
			, { Binding::createStaticConverting("@@ID@@", id.id()
					, customizer.factory()->nameNormalizerConverter()) }
			, parent)
{
}
