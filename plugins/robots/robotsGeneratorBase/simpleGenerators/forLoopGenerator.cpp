#include "forLoopGenerator.h"
#include "../generatorCustomizer.h"

using namespace qReal::robots::generators::simple;

QString const iteratorName = "__iter__";

ForLoopGenerator::ForLoopGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "conditional/for.t", QList<Binding *>()
			<< Binding::createStaticConverting("@@ITERATOR_TYPE@@", "int", customizer.factory()->typeConverter())
			<< Binding::createStatic("@@ITERATOR@@", iteratorName)
			<< Binding::createStatic("@@INITIAL_VALUE@@", "0")
			<< Binding::createConverting("@@BOUND@@", "Iterations", customizer.factory()->intPropertyConverter())
			, parent)
{
}
