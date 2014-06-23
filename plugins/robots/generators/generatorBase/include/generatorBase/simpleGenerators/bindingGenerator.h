#pragma once

#include "generatorBase/simpleGenerators/abstractSimpleGenerator.h"
#include "generatorBase/simpleGenerators/binding.h"
#include "generatorBase/robotsGeneratorDeclSpec.h"

namespace generatorBase {
namespace simple {

/// A base for all generators that use bindings and templates for generating code
class ROBOTS_GENERATOR_EXPORT BindingGenerator : public AbstractSimpleGenerator
{
public:
	BindingGenerator(qrRepo::RepoApi const &repo
			, GeneratorCustomizer &customizer
			, qReal::Id const &id
			, QString const &pathToTemplate
			, QList<Binding *> const &bindings
			, QObject *parent = 0);

	virtual ~BindingGenerator();

	virtual QString generate();

private:
	QString const mPathToTemplate;
	QList<Binding *> const mBindings;  // Takes ownership
};

}
}
