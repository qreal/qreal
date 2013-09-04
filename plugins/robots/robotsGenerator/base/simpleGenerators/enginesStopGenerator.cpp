#include "enginesStopGenerator.h"

using namespace qReal::robots::generators::simple;

EnginesStopGenerator::EnginesStopGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "enginesStop.t", QList<Binding *>(), parent)
{
}
