#include "bindingGenerator.h"

using namespace qReal::robots::generators::simple;

BindingGenerator::BindingGenerator(LogicalModelAssistInterface const &model
		, GeneratorCustomizer &customizer
		, Id const &id
		, QString const &pathToTemplate
		, QList<Binding *> const &bindings
		, QObject *parent)
	: AbstractSimpleGenerator(model, customizer, id, parent)
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
		binding->apply(mModel, mId, input);
	}

	return input;
}
