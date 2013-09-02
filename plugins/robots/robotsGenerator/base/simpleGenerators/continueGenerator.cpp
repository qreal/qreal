#include "continueGenerator.h"

using namespace qReal::robots::generators::simple;

ContinueGenerator::ContinueGenerator(LogicalModelAssistInterface const &model
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(model, customizer, id, "continue.t", QList<Binding *>(), parent)
{
}
