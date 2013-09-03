#include "whileLoopGenerator.h"
#include "../generatorCustomizer.h"

using namespace qReal::robots::generators::simple;

WhileLoopGenerator::WhileLoopGenerator(LogicalModelAssistInterface const &model
		, GeneratorCustomizer &customizer
		, Id const &id
		, bool doWhileForm
		, bool needInverting
		, QObject *parent)
	: BindingGenerator(model, customizer, id
			, doWhileForm ? "conditional/doWhile.t" : "conditional/whileDo.t"
			, QList<Binding *>() << Binding::createConverting("@@CONDITION@@", "Condition"
					, customizer.factory()->boolPropertyConverter(needInverting))
			, parent)
{
}
