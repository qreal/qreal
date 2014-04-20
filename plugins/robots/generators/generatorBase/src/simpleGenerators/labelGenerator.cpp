#include "labelGenerator.h"

#include "generatorBase/generatorCustomizer.h"

using namespace generatorBase::simple;
using namespace qReal;

LabelGenerator::LabelGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "label.t"
			, { Binding::createStaticConverting("@@ID@@", id.id()
					, customizer.factory()->nameNormalizerConverter()) }
			, parent)
{
}
