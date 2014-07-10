#include "continueGenerator.h"

using namespace generatorBase::simple;
using namespace qReal;

ContinueGenerator::ContinueGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "continue.t", QList<Binding *>(), parent)
{
}
