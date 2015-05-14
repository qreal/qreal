#include "ifElementGenerator.h"
#include <generatorBase/generatorCustomizer.h>
#include "src/ev3GeneratorCustomizer.h"

using namespace ev3::simple;
using namespace generatorBase::simple;

IfElementGenerator::IfElementGenerator(qrRepo::RepoApi const &repo
		, generatorBase::GeneratorCustomizer &customizer
		, qReal::Id const &id
		, bool elseIsEmpty
		, bool needInverting
		, QObject *parent)
	: BindingGenerator(repo, customizer, id
			, elseIsEmpty ? "conditional/if.t" : "conditional/ifElse.t"
			, QList<Binding *>() << Binding::createConverting("@@CONDITION@@", "Condition"
					, customizer.factory()->boolPropertyConverter(id, "Condition", needInverting))
				<< Binding::createStaticConverting("@@Block@@", "Block"
					, static_cast<Ev3GeneratorFactory *>(customizer.factory())->goToBlockNumberConverter())
			, parent)
{
}

