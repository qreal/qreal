#include "timerGenerator.h"

using namespace qReal::robots::generators::simple;

TimerGenerator::TimerGenerator(LogicalModelAssistInterface const &model
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(model, customizer, id, "timer.t", QList<Binding *>(), parent)
{
}
