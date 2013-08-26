#pragma once

#include <QtCore/QObject>

#include <logicalModelAssistInterface.h>

namespace qReal {
namespace robots {
namespace generators {
namespace simple {

class AbstractSimpleGenerator : public QObject
{
public:
	AbstractSimpleGenerator(LogicalModelAssistInterface const &model
			, Id const &id
			, QObject *parent = 0);
	virtual ~AbstractSimpleGenerator();

	virtual QString generate() = 0;

private:
	LogicalModelAssistInterface const &mModel;
	Id const mId;
};

}
}
}
}
