#include "initialNodeGenerator.h"

using namespace generatorBase::simple;
using namespace qReal;

InitialNodeGenerator::InitialNodeGenerator(const qrRepo::RepoApi &repo
		, GeneratorCustomizer &customizer
		, const Id &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "initialNode.t", QList<Binding *>(), parent)
{
}
