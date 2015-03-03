#pragma once

#include <QtCore/QObject>

#include <qrrepo/repoApi.h>
#include "../templateParametrizedEntity.h"

namespace generatorBase {

class GeneratorCustomizer;

namespace simple {

/// A base for all code generators generating code for one block.
class AbstractSimpleGenerator : public QObject, public TemplateParametrizedEntity
{
public:
	AbstractSimpleGenerator(const qrRepo::RepoApi &repo
			, GeneratorCustomizer &customizer
			, const qReal::Id &id
			, QObject *parent = 0);

	virtual ~AbstractSimpleGenerator();

	/// Generates and returns code using parameters specified in constructor
	virtual QString generate() = 0;

protected:
	const qrRepo::RepoApi &mRepo;
	GeneratorCustomizer &mCustomizer;

	const qReal::Id mId;
};

}
}
