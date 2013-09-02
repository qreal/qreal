#include "breakGenerator.h"

using namespace qReal;
using namespace robots::generators::simple;

BreakGenerator::BreakGenerator(LogicalModelAssistInterface const &model
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(model, customizer, id, "break.t", QList<Binding *>(), parent)
{
}
