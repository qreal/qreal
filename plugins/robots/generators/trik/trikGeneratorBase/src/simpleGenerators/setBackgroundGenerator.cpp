#include "setBackgroundGenerator.h"

#include <generatorBase/generatorCustomizer.h>
#include "src/converters/backgroundColorConverter.h"

using namespace trik::simple;
using namespace trik::converters;
using namespace generatorBase::simple;

SetBackgroundGenerator::SetBackgroundGenerator(const qrRepo::RepoApi &repo
		, generatorBase::GeneratorCustomizer &customizer
		, const qReal::Id &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id
			, "drawing/setBackground.t"
			, QList<Binding *>()
					<< Binding::createConverting(
							"@@COLOR@@"
							, "Color"
							, new BackgroundColorConverter(customizer.factory()->pathToTemplates())
					)
			, parent)
{
}
