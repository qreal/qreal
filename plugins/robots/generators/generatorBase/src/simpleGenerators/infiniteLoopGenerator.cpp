#include "infiniteLoopGenerator.h"

using namespace generatorBase::simple;
using namespace qReal;

InfiniteLoopGenerator::InfiniteLoopGenerator(const qrRepo::RepoApi &repo
		, GeneratorCustomizer &customizer
		, const Id &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "conditional/infinite.t", QList<Binding *>(), parent)
{
}
