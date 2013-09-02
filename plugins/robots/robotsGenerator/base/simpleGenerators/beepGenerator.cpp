#include "beepGenerator.h"
#include "../generatorCustomizer.h"

using namespace qReal::robots::generators::simple;

BeepGenerator::BeepGenerator(LogicalModelAssistInterface const &model
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(model, customizer, id, "beep.t", QList<Binding *>()
			<< new Binding("@@VOLUME@@", "Volume", customizer.factory()->intPropertyConverter())
			, parent)
{
}
