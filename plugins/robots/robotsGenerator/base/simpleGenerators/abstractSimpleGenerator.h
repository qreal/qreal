#pragma once

#include <QtCore/QObject>

#include <logicalModelAssistInterface.h>
#include "../templateParametrizedEntity.h"

namespace qReal {
namespace robots {
namespace generators {

class GeneratorCustomizer;

namespace simple {

class AbstractSimpleGenerator : public QObject, public TemplateParametrizedEntity
{
public:
	AbstractSimpleGenerator(LogicalModelAssistInterface const &model
			, GeneratorCustomizer &customizer
			, Id const &id
			, QObject *parent = 0);

	virtual ~AbstractSimpleGenerator();

	virtual QString generate() = 0;

protected:
	LogicalModelAssistInterface const &mModel;
	GeneratorCustomizer &mCustomizer;

	Id const mId;
};

}
}
}
}
