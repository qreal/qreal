#include "initialNodeGenerator.h"

using namespace qReal::robots::generators::simple;

InitialNodeGenerator::InitialNodeGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "initialNode.t", QList<Binding *>(), parent)
{
}
