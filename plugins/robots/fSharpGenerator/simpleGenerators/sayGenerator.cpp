#include "sayGenerator.h"

#include <generatorCustomizer.h>

using namespace qReal::robots::generators::simple;

SayGenerator::SayGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id
			, "say.t"
			, QList<Binding *>()
					<< Binding::createDirect("@@TEXT@@", "Text")
			, parent)
{
}
