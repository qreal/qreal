#include "breakGenerator.h"

using namespace generatorBase::simple;
using namespace qReal;

BreakGenerator::BreakGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "break.t", QList<Binding *>(), parent)
{
}
