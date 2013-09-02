#include "waitForSoundBlockGenerator.h"

using namespace qReal::robots::generators::simple;

WaitForSoundBlockGenerator::WaitForSoundBlockGenerator(LogicalModelAssistInterface const &model
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(model, customizer, id, "waitSound.t", QList<Binding *>(), parent)
{
}
