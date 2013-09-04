#include "bindingGenerator.h"
#include "../generatorCustomizer.h"

using namespace qReal::robots::generators::simple;

BindingGenerator::BindingGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QString const &pathToTemplate
		, QList<Binding *> const &bindings
		, QObject *parent)
	: AbstractSimpleGenerator(repo, customizer, id, parent)
	, mPathToTemplate(pathToTemplate)
	, mBindings(bindings)
{
}

BindingGenerator::~BindingGenerator()
{
	qDeleteAll(mBindings);
}

QString BindingGenerator::generate()
{
	QString input = readTemplate(mPathToTemplate);
	foreach (Binding * const binding, mBindings) {
		binding->apply(mRepo, mId, input);
	}

	return input;
}
