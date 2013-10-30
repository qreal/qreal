#include "sadSmileGenerator.h"

#include <generatorCustomizer.h>

using namespace qReal::robots::generators::simple;

SadSmileGenerator::SadSmileGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id
			, "display/sadSmile.t"
			, QList<Binding *>()
			, parent)
{
}
