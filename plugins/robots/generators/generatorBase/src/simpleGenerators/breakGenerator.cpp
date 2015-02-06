#include "breakGenerator.h"

using namespace generatorBase::simple;
using namespace qReal;

BreakGenerator::BreakGenerator(const qrRepo::RepoApi &repo
		, GeneratorCustomizer &customizer
		, const Id &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "break.t", QList<Binding *>(), parent)
{
}
