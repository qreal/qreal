#include "ledGenerator.h"

#include <generatorBase/generatorCustomizer.h>
#include "src/converters/ledColorConverter.h"

using namespace trik::simple;
using namespace trik::converters;
using namespace generatorBase::simple;

LedGenerator::LedGenerator(const qrRepo::RepoApi &repo
		, generatorBase::GeneratorCustomizer &customizer
		, const qReal::Id &id
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
