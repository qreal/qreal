#include "setBackgroundGenerator.h"

#include "generatorCustomizer.h"
#include "converters/backgroundColorConverter.h"

using namespace qReal::robots::generators::simple;
using namespace qReal::robots::generators::converters;

SetBackgroundGenerator::SetBackgroundGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id
			, "display/setBackground.t"
			, QList<Binding *>()
					<< Binding::createConverting(
							"@@COLOR@@"
							, "Color"
							, new BackgroundColorConverter(customizer.factory()->pathToTemplates())
					)
			, parent)
{
}
