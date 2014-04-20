#include "initialNodeGenerator.h"

using namespace generatorBase::simple;
using namespace qReal;

InitialNodeGenerator::InitialNodeGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "initialNode.t", QList<Binding *>(), parent)
{
}
