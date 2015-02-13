#include "whileLoopGenerator.h"
#include "generatorBase/generatorCustomizer.h"

using namespace generatorBase::simple;
using namespace qReal;

WhileLoopGenerator::WhileLoopGenerator(const qrRepo::RepoApi &repo
		, GeneratorCustomizer &customizer
		, const Id &id
		, bool doWhileForm
		, bool needInverting
		, QObject *parent)
	: BindingGenerator(repo, customizer, id
			, doWhileForm ? "conditional/doWhile.t" : "conditional/whileDo.t"
			, QList<Binding *>() << Binding::createConverting("@@CONDITION@@", "Condition"
					, customizer.factory()->boolPropertyConverter(id, "Condition", needInverting))
			, parent)
{
}
