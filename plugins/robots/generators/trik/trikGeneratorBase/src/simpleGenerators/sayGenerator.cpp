#include "sayGenerator.h"

#include <generatorBase/generatorCustomizer.h>

using namespace trik::simple;
using namespace generatorBase::simple;

SayGenerator::SayGenerator(const qrRepo::RepoApi &repo
		, generatorBase::GeneratorCustomizer &customizer
		, const qReal::Id &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id
			, "say.t"
			, { Binding::createConverting("@@TEXT@@", "Text"
					, customizer.factory()->stringPropertyConverter(id, "Text"))
			} , parent)
{
}
