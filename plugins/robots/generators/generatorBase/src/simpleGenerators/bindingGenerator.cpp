#include "generatorBase/simpleGenerators/bindingGenerator.h"

#include "generatorBase/generatorCustomizer.h"

using namespace generatorBase::simple;
using namespace qReal;

BindingGenerator::BindingGenerator(const qrRepo::RepoApi &repo
		, GeneratorCustomizer &customizer
		, const Id &id
		, const QString &pathToTemplate
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
	input.prepend(QString("@~%1~@").arg(mId.toString()));
	input.append(QString("@#%1#@").arg(mId.toString()));
	return input;
}
