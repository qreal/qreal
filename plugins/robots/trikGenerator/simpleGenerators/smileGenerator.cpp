#include "smileGenerator.h"

#include <generatorCustomizer.h>

using namespace qReal::robots::generators::simple;

SmileGenerator::SmileGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id
			, "display/smile.t"
			, QList<Binding *>()
			, parent)
{
}
