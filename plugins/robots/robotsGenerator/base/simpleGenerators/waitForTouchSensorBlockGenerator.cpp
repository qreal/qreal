#include "waitForTouchSensorBlockGenerator.h"

using namespace qReal::robots::generators::simple;

WaitForTouchSensorBlockGenerator::WaitForTouchSensorBlockGenerator(LogicalModelAssistInterface const &model
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(model, customizer, id, "waitTouch.t", QList<Binding *>(), parent)
{
}
