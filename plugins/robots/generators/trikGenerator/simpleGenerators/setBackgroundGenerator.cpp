#include "setBackgroundGenerator.h"

#include <generatorBase/generatorCustomizer.h>
#include "converters/backgroundColorConverter.h"

using namespace trik::simple;
using namespace trik::converters;
using namespace generatorBase::simple;

SetBackgroundGenerator::SetBackgroundGenerator(qrRepo::RepoApi const &repo
		, generatorBase::GeneratorCustomizer &customizer
		, qReal::Id const &id
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
