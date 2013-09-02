#include "ifElementGenerator.h"

using namespace qReal;
using namespace robots::generators::simple;

IfElementGenerator::IfElementGenerator(LogicalModelAssistInterface const &model
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(model, customizer, id, "if.t", QList<Binding *>(), parent)
{
}
