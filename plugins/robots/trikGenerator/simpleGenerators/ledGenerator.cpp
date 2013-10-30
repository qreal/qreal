#include "ledGenerator.h"

#include "generatorCustomizer.h"
#include "converters/ledColorConverter.h"

using namespace qReal::robots::generators::simple;
using namespace qReal::robots::generators::converters;

LedGenerator::LedGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
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
