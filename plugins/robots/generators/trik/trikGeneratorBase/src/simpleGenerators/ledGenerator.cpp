#include "ledGenerator.h"

#include <generatorBase/generatorCustomizer.h>
#include "src/converters/ledColorConverter.h"

using namespace trik::simple;
using namespace trik::converters;
using namespace generatorBase::simple;

LedGenerator::LedGenerator(qrRepo::RepoApi const &repo
		, generatorBase::GeneratorCustomizer &customizer
		, qReal::Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id
			, "led.t"
			, QList<Binding *>()
					<< Binding::createConverting(
								"@@COLOR@@"
								, "Color"
								, new LedColorConverter(customizer.factory()->pathToTemplates())
						)
			, parent)
{
}
