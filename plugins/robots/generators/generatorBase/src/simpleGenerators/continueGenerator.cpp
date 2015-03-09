#include "continueGenerator.h"

using namespace generatorBase::simple;
using namespace qReal;

ContinueGenerator::ContinueGenerator(const qrRepo::RepoApi &repo
		, GeneratorCustomizer &customizer
		, const Id &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "continue.t", QList<Binding *>(), parent)
{
}
