#include "whileLoopGenerator.h"
#include "generatorBase/generatorCustomizer.h"

using namespace generatorBase::simple;
using namespace qReal;

WhileLoopGenerator::WhileLoopGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, bool doWhileForm
		, bool needInverting
		, QObject *parent)
	: BindingGenerator(repo, customizer, id
			, doWhileForm ? "conditional/doWhile.t" : "conditional/whileDo.t"
			, QList<Binding *>() << Binding::createConverting("@@CONDITION@@", "Condition"
					, customizer.factory()->boolPropertyConverter(id, needInverting))
			, parent)
{
}
