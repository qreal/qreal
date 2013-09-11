#pragma once

#include <QtCore/QObject>

#include <qrrepo/repoApi.h>
#include "../templateParametrizedEntity.h"

namespace qReal {
namespace robots {
namespace generators {

class GeneratorCustomizer;

namespace simple {

class AbstractSimpleGenerator : public QObject, public TemplateParametrizedEntity
{
public:
	AbstractSimpleGenerator(qrRepo::RepoApi const &repo
			, GeneratorCustomizer &customizer
			, Id const &id
			, QObject *parent = 0);

	virtual ~AbstractSimpleGenerator();

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
