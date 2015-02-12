#include "ifElementGenerator.h"
#include "generatorBase/generatorCustomizer.h"

using namespace generatorBase::simple;
using namespace qReal;

IfElementGenerator::IfElementGenerator(const qrRepo::RepoApi &repo
		, GeneratorCustomizer &customizer
		, const Id &id
		, bool elseIsEmpty
		, bool needInverting
		, QObject *parent)
	: BindingGenerator(repo, customizer, id
			, elseIsEmpty ? "conditional/if.t" : "conditional/ifElse.t"
			, QList<Binding *>() << Binding::createConverting("@@CONDITION@@", "Condition"
					, customizer.factory()->boolPropertyConverter(id, "Condition", needInverting))
			, parent)
{
}
