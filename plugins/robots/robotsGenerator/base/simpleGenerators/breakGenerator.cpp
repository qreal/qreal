#include "breakGenerator.h"

using namespace qReal;
using namespace robots::generators::simple;

BreakGenerator::BreakGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "break.t", QList<Binding *>(), parent)
{
}
