#include "timerGenerator.h"
#include "../generatorCustomizer.h"

using namespace qReal::robots::generators::simple;

TimerGenerator::TimerGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "wait/timer.t", QList<Binding *>()
			<< Binding::createConverting("@@DELAY@@", "Delay", customizer.factory()->intPropertyConverter())
			, parent)
{
}
