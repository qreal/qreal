#include "timerGenerator.h"
#include "../generatorCustomizer.h"

using namespace qReal::robots::generators::simple;

TimerGenerator::TimerGenerator(LogicalModelAssistInterface const &model
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(model, customizer, id, "wait/timer.t", QList<Binding *>()
			<< Binding::createConverting("@@DELAY@@", "Delay", customizer.factory()->intPropertyConverter())
			, parent)
{
}
