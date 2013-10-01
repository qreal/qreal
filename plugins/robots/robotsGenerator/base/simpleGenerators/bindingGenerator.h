#pragma once

#include "robotsGeneratorDeclSpec.h"
#include "abstractSimpleGenerator.h"
#include "binding.h"

namespace qReal {
namespace robots {
namespace generators {
namespace simple {

/// A base for all generators that use bindings and templates for generating code
class ROBOTS_GENERATOR_EXPORT BindingGenerator : public AbstractSimpleGenerator
{
public:
	BindingGenerator(qrRepo::RepoApi const &repo
			, GeneratorCustomizer &customizer
			, Id const &id
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
}
}
