#include "infiniteLoopGenerator.h"

using namespace qReal::robots::generators::simple;

InfiniteLoopGenerator::InfiniteLoopGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "conditional/infinite.t", QList<Binding *>(), parent)
{
}
