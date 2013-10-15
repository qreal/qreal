#pragma once

#include <QtCore/QObject>

#include <qrrepo/repoApi.h>
#include "../templateParametrizedEntity.h"

namespace qReal {
namespace robots {
namespace generators {

class GeneratorCustomizer;

namespace simple {

/// A base for all code generators generating code for one block.
class AbstractSimpleGenerator : public QObject, public TemplateParametrizedEntity
{
public:
	AbstractSimpleGenerator(qrRepo::RepoApi const &repo
			, GeneratorCustomizer &customizer
			, Id const &id
			, QObject *parent = 0);

	virtual ~AbstractSimpleGenerator();

	/// Generates and returns code using parameters specified in constructor
	virtual QString generate() = 0;

protected:
	qrRepo::RepoApi const &mRepo;
	GeneratorCustomizer &mCustomizer;

	Id const mId;
};

}
}
}
}
