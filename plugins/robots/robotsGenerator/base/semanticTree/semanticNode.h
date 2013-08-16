#pragma once

#include <QtCore/QObject>

#include <ids.h>

namespace qReal {
namespace robots {
namespace generators {
namespace semantics {

class SemanticNode : public QObject
{
public:
	SemanticNode(Id const &idBinded = Id(), QObject *parent = 0);

	void bindTo(Id const &id);

private:
	Id mId;
};

}
}
}
}
