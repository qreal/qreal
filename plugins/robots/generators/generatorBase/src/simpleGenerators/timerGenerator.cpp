#include "timerGenerator.h"
#include "generatorBase/generatorCustomizer.h"

using namespace generatorBase::simple;
using namespace qReal;

TimerGenerator::TimerGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "wait/timer.t", QList<Binding *>()
			<< Binding::createConverting("@@DELAY@@", "Delay", customizer.factory()->intPropertyConverter(id))
			, parent)
{
}
