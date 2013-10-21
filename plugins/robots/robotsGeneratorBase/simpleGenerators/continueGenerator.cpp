#include "continueGenerator.h"

using namespace qReal::robots::generators::simple;

ContinueGenerator::ContinueGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "continue.t", QList<Binding *>(), parent)
{
}
