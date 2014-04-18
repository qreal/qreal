#include "ifElementGenerator.h"
#include "generatorBase/generatorCustomizer.h"

using namespace qReal;
using namespace robots::generators::simple;

IfElementGenerator::IfElementGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, bool elseIsEmpty
		, bool needInverting
		, QObject *parent)
	: BindingGenerator(repo, customizer, id
			, elseIsEmpty ? "conditional/if.t" : "conditional/ifElse.t"
			, QList<Binding *>() << Binding::createConverting("@@CONDITION@@", "Condition"
					, customizer.factory()->boolPropertyConverter(needInverting))
			, parent)
{
}
