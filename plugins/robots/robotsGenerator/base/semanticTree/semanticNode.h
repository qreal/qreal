#pragma once

#include <QtCore/QObject>
// TODO: remove it
#include <QtCore/QDebug>

#include <ids.h>

namespace qReal {
namespace robots {
namespace generators {
namespace semantics {

class SemanticNode : public QObject
{
public:
	void bindTo(Id const &id);

protected:
	virtual void debugPrint(int indent) = 0;

	SemanticNode(Id const &idBinded = Id(), QObject *parent = 0);

private:
	Id mId;
};

}
}
}
}
