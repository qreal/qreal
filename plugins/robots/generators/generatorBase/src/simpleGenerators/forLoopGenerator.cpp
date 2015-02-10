#include "forLoopGenerator.h"
#include "generatorBase/generatorCustomizer.h"

using namespace generatorBase::simple;
using namespace qReal;

const QString iteratorName = "__iter__";

ForLoopGenerator::ForLoopGenerator(const qrRepo::RepoApi &repo
		, GeneratorCustomizer &customizer
		, const Id &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "conditional/for.t", QList<Binding *>()
			<< Binding::createStaticConverting("@@ITERATOR_TYPE@@", "int", customizer.factory()->typeConverter())
			<< Binding::createStatic("@@ITERATOR@@", iteratorName)
			<< Binding::createStatic("@@INITIAL_VALUE@@", "0")
			<< Binding::createConverting("@@BOUND@@", "Iterations"
					, customizer.factory()->intPropertyConverter(id, "Iterations"))
			, parent)
{
}
