#include "gotoSimpleGenerator.h"

#include "generatorBase/generatorCustomizer.h"

using namespace qReal::robots::generators::simple;

GotoSimpleGenerator::GotoSimpleGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "goto.t"
			, { Binding::createStaticConverting("@@ID@@", id.id()
					, customizer.factory()->nameNormalizerConverter()) }
			, parent)
{
}
