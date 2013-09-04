#pragma once

#include <QtCore/QObject>
#include <QtCore/QLinkedList>

#include <qrkernel/ids.h>
#include "../generatorCustomizer.h"

namespace qReal {
namespace robots {
namespace generators {
namespace semantics {

class SemanticNode : public QObject
{
public:
	Id id() const;
	void bindTo(Id const &id);

	void setParentNode(SemanticNode *parent);

	virtual QString toString(GeneratorCustomizer &customizer, int indent) const = 0;

	SemanticNode *findNodeFor(Id const &id);

protected:
	SemanticNode(Id const &idBinded = Id(), QObject *parent = 0);

	virtual QLinkedList<SemanticNode *> children() const = 0;

	QString addIndent(QString const &code, int indent) const;

	Id mId;
	SemanticNode *mParentNode;
};

}
}
}
}
