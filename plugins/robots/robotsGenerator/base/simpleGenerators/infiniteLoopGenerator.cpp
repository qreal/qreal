#include "infiniteLoopGenerator.h"

using namespace qReal::robots::generators::simple;

InfiniteLoopGenerator::InfiniteLoopGenerator(LogicalModelAssistInterface const &model
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(model, customizer, id, "conditional/infinite.t", QList<Binding *>(), parent)
{
}
