#include "infiniteLoopGenerator.h"

using namespace generatorBase::simple;
using namespace qReal;

InfiniteLoopGenerator::InfiniteLoopGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "conditional/infinite.t", QList<Binding *>(), parent)
{
}
