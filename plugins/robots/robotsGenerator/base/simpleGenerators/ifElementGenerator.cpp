#include "ifElementGenerator.h"
#include "../generatorCustomizer.h"

using namespace qReal;
using namespace robots::generators::simple;

IfElementGenerator::IfElementGenerator(LogicalModelAssistInterface const &model
		, GeneratorCustomizer &customizer
		, Id const &id
		, bool elseIsEmpty
		, bool needInverting
		, QObject *parent)
	: BindingGenerator(model, customizer, id
			, elseIsEmpty ? "conditional/if.t" : "conditional/ifElse.t"
			, QList<Binding *>() << Binding::createConverting("@@CONDITION@@", "Condtion"
					, customizer.factory()->boolPropertyConverter(needInverting))
			, parent)
{
}
