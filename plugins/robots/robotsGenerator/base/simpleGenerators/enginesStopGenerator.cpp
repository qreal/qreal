#include "enginesStopGenerator.h"

using namespace qReal::robots::generators::simple;

EnginesStopGenerator::EnginesStopGenerator(LogicalModelAssistInterface const &model
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(model, customizer, id, "enginesStop.t", QList<Binding *>(), parent)
{
}
